package org.cpd2425.t09g13.sync;

import java.util.function.Function;

/**
 * MutexGuard
 */
public class MutexGuard<T> implements AutoCloseable {
  Mutex<T> inner;

  MutexGuard(Mutex<T> in) {
    in.lock();
    this.inner = in;
  }

  public T get() throws MutexOwnershipException {
    return this.inner.unsafeGet();
  }

  public void set(T v) throws MutexOwnershipException {
    this.inner.unsafeSet(v);
  }

  public void map(Function<T,T> f) throws MutexOwnershipException {
    this.inner.unsafeSet(f.apply(this.inner.unsafeGet()));
  }

  @Override
  public void close() {
    this.inner.unlock();
  }
  
}
