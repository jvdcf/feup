package org.cpd2425.t09g13.sync.channel;

/**
 * InvalidReceiverException
 */
public class InvalidReceiverException extends Exception {
  public InvalidReceiverException() {
    super("Attempted to receive from an invalidated receiver.");
  }
}
