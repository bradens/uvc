/**
 * Assignment 6
 * SEng 360
 * Cryptography and Digital Signatures
 * @author Braden Simpson
 * V00685500
 * 
 * Serializable License model class
 */
package model;

import java.io.Serializable;
import java.util.Date;

import model.Resources.LicenseType;

public class License implements Serializable {
	private static final long serialVersionUID = 1L;
	public License() { }
	private String serial;
	private LicenseType type;
	private Date expiry;
	private String ip;
	
	public License(String serial, LicenseType type, Date expiry, String ip) {
		this.serial = serial;
		this.type = type;
		this.expiry = expiry;
		this.ip = ip;
	}
	
	public String getSerial() {
		return serial;
	}
	
	public void setSerial(String serial) {
		this.serial = serial;
	}
	
	public LicenseType getType() {
		return type;
	}
	
	public void setType(LicenseType type) {
		this.type = type;
	}
	
	public Date getExpiry() {
		return expiry;
	}
	
	public void setExpiry(Date expiry) {
		this.expiry = expiry;
	}
	
	public String getIp() {
		return ip;
	}
	
	public void setIp(String ip) {
		this.ip = ip;
	}
}