/**
 * Assignment 6
 * SEng 360
 * Cryptography and Digital Signatures
 * @author Braden Simpson
 * V00685500
 * 
 * Client class.  Requests a model.License from the impl.LicenseManagerImpl,
 * checks the digital signature with the public key of the LicenseManager.  Then on
 * a successful verification, stores the license in a text file in a given directory.
 */
package impl;

import java.io.FileOutputStream;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.security.KeyFactory;
import java.security.PublicKey;
import java.security.Signature;
import java.security.spec.X509EncodedKeySpec;
import javax.xml.bind.DatatypeConverter;
import model.Resources.LicenseType;
import business.LicenseManager;

public class Client {
	private static PublicKey publicKey;

	private Client() {
	}

	/**
	 * If the user doesn't enter an argument then the license is stored in
	 * license.txt
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		if (args.length < 1) {
			System.err.println("Not enough arguments given.\n"
							+ "Usage: java Client (TRIAL | BASIC | PROFESSIONAL) [filename]");
			return;
		}
		LicenseType type = LicenseType.valueOf(args[0]);
		String filepath = (args.length == 1 ? "license.txt" : args[1]); 
		try {
			Registry registry = LocateRegistry.getRegistry(null);
			LicenseManager stub = (LicenseManager) registry
					.lookup("LicenseManager");

			// Get our public key. Can assume that this is a legit key because
			// we aren't implementing a certificate server
			X509EncodedKeySpec pubKeySpec = new X509EncodedKeySpec(
					stub.getPublicKey());
			KeyFactory keyFactory = KeyFactory.getInstance("DSA", "SUN");
			publicKey = keyFactory.generatePublic(pubKeySpec);

			byte[] signature = stub.getLicenseSignature(type);
			byte[] licenseBytes = stub.getLicenseBytes();
			Signature sig = Signature.getInstance("SHA1withDSA", "SUN");
			sig.initVerify(publicKey);
			sig.update(licenseBytes);
			boolean verifies = sig.verify(signature);
			
			if (verifies) {
				System.out.println("Successfully validated license, writing to " + filepath);
				FileOutputStream fos = new FileOutputStream(filepath);
				fos.write(DatatypeConverter.printBase64Binary(licenseBytes).getBytes());
				fos.close();
			}
			else {
				// Don't write the file and display a message
				System.err.println("Error validating license.  The license is invalid.");
				return;
			}

		} catch (Exception e) {
			System.err.println("Client exception: " + e.toString());
			e.printStackTrace();
		}
	}
}
