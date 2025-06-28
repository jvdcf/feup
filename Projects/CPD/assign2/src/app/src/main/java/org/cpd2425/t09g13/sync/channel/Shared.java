package org.cpd2425.t09g13.sync.channel;

import java.util.ArrayList;
import java.util.List;

import org.cpd2425.t09g13.sync.Mutex;
import org.cpd2425.t09g13.sync.MutexGuard;
import org.cpd2425.t09g13.sync.MutexOwnershipException;



/**
 * Channel
 */
class Shared<T> {
  public final List<Mutex<Slot<T>>> msg;
  public final Mutex<Integer> queueSize;
  public final Mutex<Integer> tail;
  // INFO: List of all receivers for this shared state.
  public final Mutex<List<Receiver<T>>> rcvRefs;

  Shared(Integer bufSize) {
    ArrayList<Mutex<Slot<T>>> slots = new ArrayList<>(bufSize);
    for (int i = 0; i < bufSize; ++i) {
      slots.add(new Mutex<>(new Slot<>()));
    }
    this.msg = slots;
    this.queueSize = new Mutex<>(bufSize);
    this.tail = new Mutex<>(0);
    this.rcvRefs = new Mutex<>(new ArrayList<>()); 
  }

  void receiverDisconnectUpdateSlots(Integer since) {
    Integer modulus = this.getModulus();
    Integer tail = this.tailPos() % modulus;
    Integer sinc = since % modulus;
    if (sinc <= tail) {
      for (int idx = since; idx < tail; idx = (idx + 1) % modulus) {
        Mutex<Slot<T>> lockedSlot = this.msg.get(idx);
        try (MutexGuard<Slot<T>> slotGuard = lockedSlot.acquireMutex()) {
          slotGuard.map((Slot<T> slot) -> {
            slot.decRem();
            return slot;
          });
        } catch (MutexOwnershipException e) {
          throw new RuntimeException(e.getMessage());
        }
      } 
    } else {  
      for (int idx = 0; idx < tail; idx = (idx + 1) % modulus) {
        Mutex<Slot<T>> lockedSlot = this.msg.get(idx);
        try (MutexGuard<Slot<T>> slotGuard = lockedSlot.acquireMutex()) {
          slotGuard.map((Slot<T> slot) -> {
            slot.decRem();
            return slot;
          });
        } catch (MutexOwnershipException e) {
          throw new RuntimeException(e.getMessage());
        }
      }
      for (int idx = sinc; idx < modulus; idx = (idx + 1) % modulus) {
        Mutex<Slot<T>> lockedSlot = this.msg.get(idx);
        try (MutexGuard<Slot<T>> slotGuard = lockedSlot.acquireMutex()) {
          slotGuard.map((Slot<T> slot) -> {
            slot.decRem();
            return slot;
          });
        } catch (MutexOwnershipException e) {
          throw new RuntimeException(e.getMessage());
        }
      }        
    }
  }

  Integer receiverCount() {
    try (MutexGuard<List<Receiver<T>>> rcvGuard = this.rcvRefs.acquireMutex()) {
      return rcvGuard.get().size();
    } catch (MutexOwnershipException e) {
      throw new RuntimeException(e.getMessage());
    }
  }

  Integer tailPos() {
    // SAFETY: Does not reference internal state.
    try (MutexGuard<Integer> tailGuard = this.tail.acquireMutex()) {
      return tailGuard.get() + 0;
    } catch (MutexOwnershipException e) {
      throw new RuntimeException(e.getMessage());
    } 
  }

  void incrementTail() {
    try (
      MutexGuard<Integer> tailGuard = this.tail.acquireMutex();
      MutexGuard<Integer> queueGuard = this.queueSize.acquireMutex();
    ) {
      if (tailGuard.get().equals(Integer.min(Integer.MAX_VALUE, queueGuard.get() * 3))) {
        tailGuard.set(0);
      } else {
        tailGuard.set(tailGuard.get() + 1);
      }
    } catch (MutexOwnershipException e) {
      throw new RuntimeException(e.getMessage());
    } 
  }

  Integer getModulus() {
    try (MutexGuard<Integer> queueGuard = this.queueSize.acquireMutex()) {
      return queueGuard.get();
    } catch (MutexOwnershipException e) {
      throw new RuntimeException(e.getMessage());
    } 
  }

  Receiver<T> newReceiver() {
      Integer pos = this.tailPos();
      Receiver<T> res = new Receiver<>(this, pos);
      try (MutexGuard<List<Receiver<T>>> rcvsGuard = this.rcvRefs.acquireMutex()) {
        List<Receiver<T>> lst = rcvsGuard.get();
        lst.add(res);
        rcvsGuard.set(lst);
      } catch (MutexOwnershipException e) {
      throw new RuntimeException(e.getMessage());
      }
      return res; 
  }

  void notifyReceivers() {
    try (MutexGuard<List<Receiver<T>>> rcvsGuard = this.rcvRefs.acquireMutex()) {
      List<Receiver<T>> rcvs = rcvsGuard.get();
      for (Receiver<T> receiver : rcvs) {
        synchronized (receiver) {
          receiver.notifyAll();
        }
      }
      rcvsGuard.set(rcvs);
    } catch (MutexOwnershipException e) {
      throw new RuntimeException(e.getMessage());
    }
  }
}
