
import java.io.*;
import java.util.*;
import java.net.*;

public class Server extends Thread{
	ServerSocket inSock;
	Socket server;
	int port = 44347;
	HashMap<String, InetSocketAddress> connectPairs = new HashMap<String, InetSocketAddress>();
	
	public static void main(String[] args){
		Server serve = new Server();
		serve.run();
	}
	
	public Server(){
		try{
			inSock = new ServerSocket(port);
			//run();
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
		
		DataOutputStream cOutStream = null;
		BufferedReader cInReader = null;
		
		Socket clientSock = null;
		
		try {
			System.out.println("Waiting for data");
			server = inSock.accept();
			inReader = new BufferedReader(new InputStreamReader(server.getInputStream()));
			inStream = new DataInputStream(server.getInputStream());
			outStream = new DataOutputStream(server.getOutputStream());
			System.out.println("Opened stream");
			//System.out.println(inReader.readLine());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		String in = null;
		String user = "";
		char[] cbuf = new char[50];
		while(true){
			try{
				
//				System.out.println("Getting input");
//				in = inStream.readLine();
//				in = inReader.readLine();
//				System.out.println(in);
				//System.out.println("Looping for input");
				//System.out.println(inStream.readLine());	
				while(in == null) 
					in = inReader.readLine();
				in = in.substring(2, in.length());
//				System.out.println(in);
				if(in.equals("list")){
					outStream.writeUTF(connectPairs.keySet() + "\n");
					in = null;
					continue;
				}
				String[] tok = in.split(",");
				
				user = tok[0];
				connectPairs.put(user, new InetSocketAddress(server.getInetAddress(), new Integer(tok[1])));
				clientSock = new Socket(server.getInetAddress(), new Integer(tok[1])); 
//				inStream = new DataInputStream(clientSock.getInputStream());
				cOutStream = new DataOutputStream(clientSock.getOutputStream());
				cOutStream.writeUTF(user + "," + connectPairs.get(user) + "\n");
				in = null;
//				clientSock.close();
			} catch( IOException e){
				
			}
			
		}
	}
	
}
