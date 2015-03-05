

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class lab3 extends Thread {
	public static void main(String[] args) throws IOException {
		serverClass server = new serverClass();
		server.port = 55555;
		Thread t1 = new Thread(server);
		t1.start();
		while(true){
			InputStreamReader str= new InputStreamReader (System.in);
			BufferedReader uinp= new BufferedReader (str);
			System.out.println("Start client ? type JAVA for java server and C++ for c++ server");
			String buffer =uinp.readLine();
			if (buffer.equals("JAVA")){
				clientClass client = new clientClass();
				client.port = 55555;
				Thread t2 = new Thread(client);
				t2.start();
			}
			else {
				clientClass client = new clientClass();
				client.port = 55556;
				Thread t2 = new Thread(client);
				t2.start();
			}
		}
	}
}
