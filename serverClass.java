

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class serverClass extends Thread{
	ServerSocket listener;
	int port;
	Socket sock;
	@SuppressWarnings("unused")
	public void run () {
		System.out.println("Server : First run, starting thread. ID " + Thread.currentThread().getId());
		try {
			this.listener = new ServerSocket(this.port);
		while(true){
			System.out.println("Server : Waiting for new connection.");
			this.sock = listener.accept();
			System.out.println("Server : Connection established.");
			sendmsgToclient("Server : Welcome to Java sever. Enter filename.");
			String filename = receiveMsg();
			System.out.println("client : filename = " + filename);
	        File myFile = new File (filename);
	        byte[] mybytearray  = new byte [(int)myFile.length()];
	        FileInputStream fis;
	        if ((fis = new FileInputStream(myFile)) != null){
	        	System.out.println("Server : Sending file.");
	        	BufferedInputStream bis = new BufferedInputStream(fis);
		        bis.read(mybytearray,0,mybytearray.length);
		        OutputStream os = sock.getOutputStream();
		        os.write(mybytearray,0,mybytearray.length);
		        os.flush();
		        bis.close();
		        System.out.println("Server : file sent, going to idle state");
	        }
	        else
	        	System.out.println("Server : File not found.");
		}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	void sendmsgToclient (String msg) throws IOException {
		OutputStream out = sock.getOutputStream();
		out = new BufferedOutputStream(out);
		PrintWriter pw = new PrintWriter(new OutputStreamWriter(out));
		pw.println(msg);
		pw.flush();out.flush();
	}
	String receiveMsg () throws IOException {
		InputStream in = sock.getInputStream();
		BufferedReader br = new BufferedReader(new InputStreamReader(in));
		return br.readLine();
	}
}
