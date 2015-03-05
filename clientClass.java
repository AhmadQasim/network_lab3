

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;

public class clientClass extends Thread{
	int port;
	Socket sock;
	public void run () {
			try {
				this.sock = new Socket("127.0.0.1", this.port);
				byte [] mybytearray  = new byte [5035];
				InputStream is = sock.getInputStream();
				System.out.println(receiveMsg());
				sendmsgToServer("file.txt");
				FileOutputStream fos = new FileOutputStream("file(client-copy).txt");
				BufferedOutputStream bos = new BufferedOutputStream(fos);
				System.out.println("client : Receiving file");
				int bytesRead = is.read(mybytearray,0,mybytearray.length);
				int current = bytesRead;
				bos.write(mybytearray, 0 , current);
				bos.flush();
				bos.close();
				sock.close();
				System.out.println("client : file received. Going offline. ID " + Thread.currentThread().getId());
			} catch (IOException e) {
				e.printStackTrace();
			}
	}
	void sendmsgToServer (String msg) throws IOException {
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
