/**
 * Braden Simpson
 * Assignment 4, SEng 360
 * Part1.java
 */

package main;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import sun.misc.BASE64Decoder;

public class Part1 {
  public static String key = "Have you failed?";
  public static String iv = "This is your IV!";
  public static String text64enc = "BvHOf2T1uj9p29LxWrx6BnFPaNANDNi0efFoDyNK400oOORiel0KQFFikE52R1gkuZsfYG17tzJqyuOp20HSGrSVCiQP3r9PawdWu2dLu6BE2ZM5lAvBnB/l4QFGEbiiAsjyUIDCZ1gT69+CDa/HXQ==";

  public static void runPart1() throws Exception
  {
    // Setup the decrypter.
    SecretKeySpec keyspec = new SecretKeySpec(key.getBytes(), "AES");
    IvParameterSpec ivspec = new IvParameterSpec(iv.getBytes());
    Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding"); 
    cipher.init(Cipher.DECRYPT_MODE, keyspec, ivspec); 
    
    byte[] decodedData = new BASE64Decoder().decodeBuffer(text64enc);
    byte[] decryptedData = cipher.doFinal(decodedData);
    System.out.println(new String(decryptedData));
  }

}
