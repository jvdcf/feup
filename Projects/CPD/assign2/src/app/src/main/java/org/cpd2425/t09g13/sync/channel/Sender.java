package org.cpd2425.t09g13.sync.channel;

import java.util.List;

import org.cpd2425.t09g13.sync.Mutex;
import org.cpd2425.t09g13.sync.MutexGuard;
import org.cpd2425.t09g13.sync.MutexOwnershipException;

/**
 * Sender
 */
public class Sender<T> {
  private final Shared<T> shared;

  public Sender(Integer bufSize) {
    this.shared = new Shared<>(bufSize);
  }
  
  Sender(Shared<T> in) {
    this.shared = in;
  }

  public void send(T msg) {
    Integer modulus = shared.getModulus();
    Integer tail = shared.tailPos();
    Integer idx = tail % modulus;
    Mutex<Slot<T>> lockedSlot = this.shared.msg.get(idx); 
    try (MutexGuard<Slot<T>> slotGuard = lockedSlot.acquireMutex()) {
      Slot<T> nextMsg = slotGuard.get();
      try (MutexGuard<List<Receiver<T>>> rcvGuard = shared.rcvRefs.acquireMutex()) {
        List<Receiver<T>> rcvs = rcvGuard.get();
        if (nextMsg.remaining() > 0) {
          for (Receiver<T> rcv : rcvs) {
            if (idx.equals(rcv.getPos() % modulus) && !tail.equals(rcv.getPos())) {
              rcv.invalidate();
            }
          }
          rcvs.removeIf(i -> i.isInvalid());
        }
        nextMsg.set(msg, rcvs.size(), tail);
        rcvGuard.set(rcvs);
      }
      slotGuard.set(nextMsg);
    } catch (MutexOwnershipException e) {
      throw new RuntimeException(e.getMessage());
    }
    this.shared.msg.set(idx, lockedSlot);
    // Move tail
    shared.incrementTail();
    
    this.notifyReceivers();
  }

  public Sender<T> clone() {
    return new Sender<>(this.shared);    
  }

  public Receiver<T> newReceiver() {
    return this.shared.newReceiver();    
  }

  private void notifyReceivers() {
    this.shared.notifyReceivers();
  } 
}
