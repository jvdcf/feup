package org.cpd2425.t09g13.utils;

/**
 * Panic
 */
public class Panic {
  private Panic() {}
  
  public static RuntimeException unreachable() {
    return unreachable("Unreachable Expression");
  }
  
  public static RuntimeException unreachable(String msg)  {
    return new RuntimeException("Unreachable: " + msg);
  }

  public static RuntimeException panic(String msg) {
    return new RuntimeException("PANIC: " + msg);
  }

  public static RuntimeException todo(String msg) {
    return new RuntimeException("You idiot! TODO: " + msg);
  }
}
