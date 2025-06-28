package org.cpd2425.t09g13.utils;

import java.util.function.Function;

/**
 * Pair
 */
public record Pair<T,U>(T left, U right) {
  public <R> Pair<R,U> mapLeft(Function<T, R> f) {
    U right = this.right();
    R left = f.apply(this.left());
    return new Pair<>(left, right);
  }

  public <R> Pair<T,R> mapRight(Function<U, R> f) {
    R right = f.apply(this.right());
    T left = this.left();
    return new Pair<>(left, right);
  }
  
}
