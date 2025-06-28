package org.cpd2425.t09g13.sync;

/**
 * MutexOwnershipException
 */
public class MutexOwnershipException extends Exception {  
  public MutexOwnershipException() {
    super("Attempted to do Mutex operations without owning the lock.");
  }
}
