package org.cpd2425.t09g13.sync.channel;

import java.util.Optional;

import org.cpd2425.t09g13.sync.Cell;
import org.cpd2425.t09g13.sync.Mutex;
import org.cpd2425.t09g13.sync.MutexGuard;
import org.cpd2425.t09g13.sync.MutexOwnershipException;

/**
 * Receiver
 */
public class Receiver<T> {
  private Shared<T> shared;    
  private Cell<Integer> pos;
  // INFO: This is important.
  private boolean isInvalid;

  Receiver(Shared<T> shared, Integer pos) {
    this.shared = shared;
    this.pos = new Cell<>(pos);
    this.isInvalid = false;
  }

  public void waitForMsg(Integer timeOut) {
    try {
      synchronized(this) {
        this.wait(timeOut);
      }
    } catch (InterruptedException e) {
    }
  }

  public void waitForMsg() {
    try {
      synchronized(this) {
        this.wait();
      }
    } catch (InterruptedException e) {
    }
  }
  
  public Optional<T> receive() throws InvalidReceiverException {
    if (this.isInvalid) {
      throw new InvalidReceiverException();
    }
    Integer modulus = this.shared.getModulus();
    Integer idx = this.pos.get() % modulus;
    Mutex<Slot<T>> lockedSlot = this.shared.msg.get(idx);
    try (MutexGuard<Slot<T>> slotGuard = lockedSlot.acquireMutex()) {
      Slot<T> s = slotGuard.get();
      if (!s.getPos().equals(this.pos.get())) {
        return Optional.empty();
      }
      Optional<T> msg = s.getMsg();
      this.incrementPos(modulus);
      s.decRem();
      slotGuard.set(s);
      this.shared.msg.set(idx, lockedSlot);
      return msg;
    } catch (MutexOwnershipException e) {
      throw new RuntimeException(e.getMessage());
    } 
  }
  
  public void invalidate() {
    this.isInvalid = true;
  }

  public boolean isInvalid() {
    return this.isInvalid;
  }

  Integer getPos() {
    return pos.get();
  }

  private void incrementPos(int modulus) {
    if (this.pos.get().equals(Integer.min(Integer.MAX_VALUE, modulus * 3))) {
      this.pos.set(0);
    } else {
      this.pos.map(i -> i + 1);
    }    
  }

  public Receiver<T> clone() {
    return this.shared.newReceiver(); 
  }
}
