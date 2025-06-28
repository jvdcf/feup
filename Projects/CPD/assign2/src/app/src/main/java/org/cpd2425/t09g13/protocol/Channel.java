package org.cpd2425.t09g13.protocol;

import java.util.Optional;

public record Channel(String name, Optional<String> prompt) {
  public boolean isAi() {
    return this.prompt().isPresent();
  }
}
