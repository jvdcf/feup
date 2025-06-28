package org.cpd2425.t09g13.sync.channel;

import java.util.Optional;

import org.cpd2425.t09g13.sync.Mutex;
import org.cpd2425.t09g13.sync.MutexGuard;
import org.cpd2425.t09g13.sync.MutexOwnershipException;


/**
 * Slot
 */
public class Slot<T> {
  private Mutex<Optional<T>> inner;
  private Mutex<Integer> rem;
  private Mutex<Integer> pos;

  Slot() {
    this.inner = new Mutex<>(Optional.empty());
    this.rem = new Mutex<>(0);
    this.pos = new Mutex<Integer>(Integer.MIN_VALUE);
  }

  Integer remaining() {
    try (MutexGuard<Integer> guard = this.rem.acquireMutex()) {
      return guard.get();
    } catch (MutexOwnershipException e) {
      throw new RuntimeException(e.getMessage());
    }
  }

  void decRem() {
    try (MutexGuard<Integer> guard = this.rem.acquireMutex()) {
      Integer rem = guard.get() - 1;
      guard.set(rem);
      if (rem.equals(0)) {
        try (MutexGuard<Optional<T>> in = this.inner.acquireMutex()) {
          in.set(Optional.empty());
        } catch (MutexOwnershipException e) {
          throw new RuntimeException(e.getMessage());
        }
      }
    } catch (MutexOwnershipException e) {
      throw new RuntimeException(e.getMessage());
    } 
  }

  Integer getPos() {
    try (MutexGuard<Integer> guard = this.pos.acquireMutex()) {
      return guard.get() + 0;
    } catch (MutexOwnershipException e) {
      throw new RuntimeException(e.getMessage());
    }
  } 

  void set(T obj, Integer rcvCount, Integer pos) {
    try (
    MutexGuard<Integer> remGuard = this.rem.acquireMutex();
    MutexGuard<Integer> posGuard = this.pos.acquireMutex();
    MutexGuard<Optional<T>> innerGuard = this.inner.acquireMutex();
    ) {
      innerGuard.set(Optional.of(obj));
      remGuard.set(rcvCount);
      posGuard.set(pos);
    } catch (MutexOwnershipException e) {
      throw new RuntimeException(e.getMessage());
    }
  }

  Optional<T> getMsg() {
    try (MutexGuard<Optional<T>> innerGuard = this.inner.acquireMutex()) {
      return innerGuard.get();
    } catch (MutexOwnershipException e) {
      throw new RuntimeException(e.getMessage());
    }
  }
 
}
