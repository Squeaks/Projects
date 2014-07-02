import java.io.*;
import java.net.*;
import java.util.*;

public class Client {
	int port = 44347;
	static int servPort = 45003;
//	String host = "127.0.0.1";
	
	Socket server;
	ServerSocket serSock;
	public Client(){
		try {
			InetAddress host = null;
			try {
				 host = InetAddress.getLocalHost();
			} catch(UnknownHostException e){
				 
			}
			server = new Socket("192.168.1.104", port);
			serSock = new ServerSocket(servPort);
//			server = new Socket();
//			server.connect(new InetSocketAddress(host, port));
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	Socket getSocket(){
		return server;
	}
	
	ServerSocket getServeSock(){
		return serSock;
	}
	
	public static void main(String[] args){
		
		
		Client con = new Client();
	
		
		DataOutputStream servOutStream = null;
		BufferedReader inReader = null;
		DataOutputStream servOut = null;
		DataOutputStream incSockOutStream = null;
		BufferedReader servIn = null;
		BufferedReader incSockIn = null;
		Socket incSock = null;
		Scanner scan;
		
//		scan = new Scanner(System.in);
		
		try {
		
			 servOutStream = new DataOutputStream(con.getSocket().getOutputStream());
			 inReader = new BufferedReader(new InputStreamReader(con.getSocket().getInputStream())); 
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		PrintWriter out = null;
		PrintStream pOut = null;
		try {
			out = new PrintWriter(con.getSocket().getOutputStream(), true);
			pOut = new PrintStream(con.getSocket().getOutputStream());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		String input = "";
		//input = scan.nextLine();
		
		//con.getSocket().
//		while(true){
			try {
				
//				if(input.equals("//quit"))
//					break;
				//outStream.writeChars("test1");
				//outStream.writeBytes("test");
				servOutStream.writeUTF("" + "Squeaks" + "," + servPort + "\n");
				incSock = con.getServeSock().accept();
				incSockOutStream = new DataOutputStream(incSock.getOutputStream());
				incSockIn = new BufferedReader(new InputStreamReader(incSock.getInputStream()));
				input = null;
				while(input == null)
					input = incSockIn.readLine();
				System.out.println("Client: " + input);
				servOutStream.writeUTF("list\n");
				
				while(input == null)
					input = incSockIn.readLine();
				input = input.substring(2, input.length());
				System.out.println(input);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
//		}
		
		try {
			servOutStream.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
//		out.print("" + con.getSocket().getLocalSocketAddress() + "," + (con.getSocket().getLocalPort() + 1));
		out.close();
		//serve.run();
		//scan = new Scanner(con.getSocket().getOutputStream());
		
	}
}
