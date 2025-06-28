package org.cpd2425.t09g13.sync;

import java.util.function.Function;

/**
 * Cell
 */
public class Cell<T> {
  private T inner;

  public Cell(T obj) {
    this.inner = obj;
  }

  public void map(Function<T,T> f) {
    this.set(f.apply(this.get()));
  }
    
  public T get() {
  	return inner;
  }

  public void set(T inner) {
  	this.inner = inner;
  }

  
}
