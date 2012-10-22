/**
 * Braden Simpson
 * Assignment 4, SEng 360
 * Part1.java
 */

package main;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;

public class Part2 {
  public static byte[] ivbytes = {(byte)0x31,(byte)0x32,(byte)0x33,(byte)0x34,(byte)0x35,(byte)0x36,(byte)0x37,(byte)0x38,(byte)0x39,(byte)0x31,(byte)0x30,(byte)0x31,(byte)0x31,(byte)0x31,(byte)0x32,(byte)0x33};
  public static byte[] keybytes = { (byte)0x86, (byte)0xe5, (byte)0x30, (byte)0x90, (byte)0xff, (byte)0x62,(byte)0xa0,(byte)0x9a,(byte)0x81,0x00,(byte)0xad,(byte)0x9e,(byte)0x8f,Byte.MIN_VALUE,Byte.MIN_VALUE,Byte.MIN_VALUE};
  public static SecretKeySpec keyspec = new SecretKeySpec(keybytes, "AES");
  public static IvParameterSpec ivspec = new IvParameterSpec(ivbytes);
  public static String prefix = "Mary had a crypto key";
  public static String encodedText = "dm8cfvs+c7pKM+WR+fde8b06SB+lqWLS4sZW+PfQSKtTfgPknzYzpTVOtJP3JBoU2Uo/7XWopjoPDOlPr24duuck0z+vAx91bYTwQo4INnIIBkj/lhJMWmvAKaUIO3qzBoGg8ynQOhuG6LY7Wo0uww==";
  public static Cipher cipher;
  
  public static void runPart2() throws Exception {
    cipher = Cipher.getInstance("AES/CBC/PKCS5Padding"); 
    byte[] decodedData = new BASE64Decoder().decodeBuffer(encodedText);
    boolean matched = false;
    String decrypted = null;
    int countIncrements = 0;
    while(!matched) {
      cipher.init(Cipher.DECRYPT_MODE, keyspec, ivspec); 
      decrypted = decrypt(decodedData);
      if (decrypted.startsWith(prefix)){
        matched = true;
        break;
      }
      else {
        // inc the key and reinit cipher
        countIncrements++;
        if (countIncrements > 16581375) {
          System.out.println("Failed to find key");
          return;
        }
        incrementAtIndex(keybytes, keybytes.length - 1);
        keyspec = new SecretKeySpec(keybytes, "AES");
        cipher = Cipher.getInstance("AES/CBC/PKCS5Padding"); 
      }
    }
    System.out.println("Try : " + countIncrements);
    System.out.println("Found matching key: " + new String(new BASE64Encoder().encodeBuffer(keybytes)));
    System.out.println("Decrypted text:" + decrypted);
  }
  
  /**
   * Helper function to increment bytes of the an array recursively.
   * @param array
   * @param index
   */
  private static void incrementAtIndex(byte[] array, int index) {
    if (array[index] == Byte.MAX_VALUE) {
        array[index] = Byte.MIN_VALUE;
        if(index > 0)
            incrementAtIndex(array, index - 1);
    }
    else {
        array[index]++;
    }
  } 
  
  /**
   * Function to perform a decryption on string
   * @see #keybytes
   * @see #ivbytes
   * @param decodedData
   * @return
   * @throws IllegalBlockSizeException
   * @throws BadPaddingException
   */
  public static String decrypt(byte[] decodedData) throws IllegalBlockSizeException {
    try {
      byte[] decryptedData = cipher.doFinal(decodedData);
      return new String(decryptedData);
    }
    catch (BadPaddingException e)
    {
      return "";
    }
  }
}
