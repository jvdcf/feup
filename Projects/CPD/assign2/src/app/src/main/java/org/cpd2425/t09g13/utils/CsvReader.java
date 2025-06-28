package org.cpd2425.t09g13.utils;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Csv
 */
public class CsvReader {
  private final List<String> header;
  private final RandomAccessFile file;

  public CsvReader(String[] header, RandomAccessFile file) throws IOException {
    List<String> headerList = Arrays.asList(header);
    this.file = file;
    try {
      String head = this.file.readLine();
      List<String> file_fields = Arrays.asList(head.split(",")).stream().map(x -> x.trim()).toList();
      if (file_fields.size() != headerList.size()) {
        throw new IOException("Header sizes don't match.");
      }
      for (String field : header) {
        if (!file_fields.contains(field)) {
          throw new IOException("Invalid Csv Header.");
        }
      }
      this.header = file_fields;
      this.file.seek(0);
    } catch (Exception e) {
      throw new IOException("Invalid Csv file: " + e);
    } 
  }

  public List<String> fields() {
    return new ArrayList<>(this.header);
  }

  public List<Map<String,String>> lookup(String field, String regex) throws IOException, IndexOutOfBoundsException {
    try {
      if (!this.header.contains(field)) {
        throw new IndexOutOfBoundsException();
      }
      // Skip header.
      this.file.readLine();
      List<Map<String,String>> result = new ArrayList<>();
      Map<String,String> curr_fields;
      String line;
      while (true) {
        try {
          line = this.file.readLine();
          if (line == null) break;  // EOF
        } catch (IOException e) {
          break;
        }
        List<String> lineFields = Arrays.asList(line.split(",")).stream().map(x -> x.trim()).toList();
        curr_fields = new HashMap<>();
        for (int i = 0; i < this.header.size(); ++i) {
          curr_fields.put(this.header.get(i), lineFields.get(i));
        }
        if (curr_fields.get(field).matches(regex)) {
          result.add(curr_fields);
        }
      }
      return result; 
    } finally {
      this.file.seek(0);
    }
  }

  public void insert(Map<String, String> entry) throws InvalidParameterException, IOException {
    if (entry.size() != this.header.size()) {
      throw new InvalidParameterException("Entry not up to spec.");
    }
    for (String field : entry.keySet()) {
      if (!this.header.contains(field)) {
        throw new InvalidParameterException(field + ": Unknown field");
      }
    }
    try {
      long pos = this.file.length();
      this.file.seek(pos);
      for (String field : this.header) {
        this.file.write((entry.get(field) + ",").getBytes()); 
      }
      this.file.write("\n".getBytes());
    } finally {
      this.file.seek(0);      
    }
  }  
}
