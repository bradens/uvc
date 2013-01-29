/**
 * Assignment 6
 * SEng 360
 * Cryptography and Digital Signatures
 * @author Braden Simpson
 * V00685500
 * 
 * This class is the implementation of the @see LicenseManager which 
 * will generate keys, and digitally sign licenses for the client
 * programs.  All communication is done through Java RMI
 */

package impl;

import java.io.ByteArrayOutputStream;
import java.io.ObjectOutput;
import java.io.ObjectOutputStream;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.Signature;
import java.util.Date;

import business.LicenseManager;

import model.License;
import model.Resources.LicenseType;

public class LicenseManagerImpl implements LicenseManager {
	private static PublicKey publicKey;
	private static PrivateKey privateKey;
	private static byte[] currentLicenseBytes;

	public LicenseManagerImpl() {
	}

	/**
	 * Generate the key pair
	 */
	public static void generateKeys() {
		try {
			System.out.println("Generating public / private key pair");
			KeyPairGenerator keyGen = KeyPairGenerator
					.getInstance("DSA", "SUN");
			SecureRandom random = SecureRandom.getInstance("SHA1PRNG", "SUN");
			keyGen.initialize(1024, random);
			KeyPair keypair = keyGen.genKeyPair();
			PrivateKey privKey = keypair.getPrivate();
			PublicKey pubKey = keypair.getPublic();
			privateKey = privKey;
			publicKey = pubKey;
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Make the License object and sign it. Then return the bytes for the
	 * signature.
	 */
	@SuppressWarnings("deprecation")
	public byte[] getLicenseSignature(LicenseType type) {
		try {
			Signature dsa = Signature.getInstance("SHA1withDSA", "SUN");
			dsa.initSign(privateKey);

			// Instantiate a 3-month license
			License newLic = new License("1", type, new Date(
					new Date().getYear(), new Date().getMonth() + 3,
					new Date().getDate()), "192.168.0.1");

			ByteArrayOutputStream bos = new ByteArrayOutputStream();
			ObjectOutput out = null;
			out = new ObjectOutputStream(bos);
			out.writeObject(newLic);
			byte[] data = bos.toByteArray();

			// Update the license bytes
			currentLicenseBytes = data;
			dsa.update(data);
			byte[] realSig = dsa.sign();
			return realSig;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	/**
	 * @returns byte array of the License object
	 */
	public byte[] getLicenseBytes() {
		return currentLicenseBytes;
	}

	/**
	 * @returns encoded public key bytes
	 */
	public byte[] getPublicKey() {
		return publicKey.getEncoded();
	}

	public static void main(String args[]) {
		try {
			LicenseManagerImpl obj = new LicenseManagerImpl();
			LicenseManager stub = (LicenseManager) UnicastRemoteObject
					.exportObject(obj, 0);

			// Bind the remote object's stub in the registry
			Registry registry = LocateRegistry.getRegistry();
			registry.bind("LicenseManager", stub);

			// Generate the key pair
			generateKeys();

			System.out.println("Server ready");
		} catch (Exception e) {
			System.err.println("Server exception: " + e.toString());
			e.printStackTrace();
		}
	}
}
