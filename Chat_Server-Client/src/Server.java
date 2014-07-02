
import java.io.*;
import java.util.*;
import java.net.*;

public class Server extends Thread{
	ServerSocket inSock;
	Socket server;
	int port = 44346;
	HashMap<SocketAddress, Integer > connectPairs = new HashMap<SocketAddress, Integer>();
	
	public Server(){
		try{
			inSock = new ServerSocket();
			inSock.bind(new InetSocketAddress(InetAddress.getLocalHost(), port));
		} catch (IOException e){
			e.printStackTrace();
			System.out.println("Could not open server socket");
			System.exit(1);
		}
		
	}
	
	public void run(){
		DataInputStream inStream = null;
		BufferedReader inReader = null;
		DataOutputStream outStream = null;
		try {
			System.out.println("Waiting for data");
			server = inSock.accept();
			inReader = new BufferedReader(new InputStreamReader(server.getInputStream()));
			inStream = new DataInputStream(server.getInputStream());
			outStream = new DataOutputStream(server.getOutputStream());
			System.out.println("Opened stream");
			System.out.println(inReader.readLine());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		while(true){
			try{
				//System.out.println("Looping for input");
				System.out.println(inStream.readLine());	
				 
				//String input = inReader.readLine();
				//System.out.println(input);
				//String[] tok = input.split(",");
				//connectPairs.put(server.getRemoteSocketAddress(), new Integer(tok[1]));
				outStream.writeChars("Input rec");
			} catch( IOException e){
				
			}
			
		}
	}
	
}
