package com.test;

public class ThreadNumber {
	private int number;

	public ThreadNumber(){
		
	}
	public ThreadNumber(int number){
		this.number = number;
	}
	public int getNumber() {
		return number;
	}

	public void setNumber(int number) {
		this.number = number;
	}
//	 public native void setNumber(int number);
//	 public native int getNumber() ;
//	 private native synchronized void fromJNI(int i);
}
