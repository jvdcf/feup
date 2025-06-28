package org.cpd2425.t09g13.sync;

import java.util.concurrent.locks.ReentrantLock;

/**
 * Mutex
 */
public class Mutex<T> extends ReentrantLock implements AutoCloseable {
  private final Cell<T> inner; 

  public Mutex(T obj) {
    this.inner = new Cell<T>(obj);
  }

  public MutexGuard<T> acquireMutex() {
    return new MutexGuard<>(this);
  }

  protected T unsafeGet() throws MutexOwnershipException {
    if (this.isLocked() && this.isHeldByCurrentThread()) {
      return this.inner.get();
    } else {
      throw new MutexOwnershipException();
    }
  }

  protected void unsafeSet(T in) throws MutexOwnershipException {
    if (this.isLocked() && this.isHeldByCurrentThread()) {
      this.inner.set(in);
    } else {
      throw new MutexOwnershipException();
    }
  }

  @Override
  public void close() {
    this.unlock();
  }
  
}
