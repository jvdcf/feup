package org.cpd2425.t09g13.utils;

import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Hasher
 */
public class Hasher {
  private Hasher() {}

  public static String hashToHex(String val) throws NoSuchAlgorithmException {
    MessageDigest digest = MessageDigest.getInstance("SHA-256");
    byte[] encodedHash = digest.digest(val.getBytes(StandardCharsets.UTF_8));
    StringBuilder sb = new StringBuilder();
    for (byte ch : encodedHash) {
      String hexDigits = Integer.toHexString(ch & 0xFF);
      if (hexDigits.length() == 1) {
        sb.append('0');
      }
      sb.append(hexDigits);
    }
    return sb.toString();
  }  
}
