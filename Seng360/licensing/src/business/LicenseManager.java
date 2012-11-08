package business;

import java.rmi.Remote;
import java.rmi.RemoteException;

import model.Resources.LicenseType;

public interface LicenseManager extends Remote {
	byte[] getLicenseSignature(LicenseType type) throws RemoteException;

	byte[] getPublicKey() throws RemoteException;

	byte[] getLicenseBytes() throws RemoteException;
}
