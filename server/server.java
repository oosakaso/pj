import java.io.*;
import java.net.*;

public class server {
  public static ServerSocket socketserver;
  public static int port = 8888;

  public static boolean start(){
    try{
      socketserver = new ServerSocket(port);
    } catch(IOException e) {
      System.out.println("Failed to start Socket server");
      e.printStackTrace();
      return false;
    }
    System.out.println("Socket server started");
    return true;
  }

  public static void listen(){
    try {
      Socket client = socketserver.accept();
      BufferedInputStream in = new BufferedInputStream(client.getInputStream());
      byte[] b = new byte[1024];
      String data = "";
      int length;
      while ((length = in.read(b)) > 0){
        data += new String(b, 0, length);
      }
      System.out.println("Data Received: " + data);
      in.close();
      in = null;
    } catch (IOException e) {
      System.out.println("Failed to Receive data");
      e.printStackTrace();
    }
  }

  public static void main(String[] args) throws IOException {

    if(start()){
      InetAddress addr = InetAddress.getLocalHost();
      System.out.println(addr);
      System.out.println("Waiting for client connection request...");
      listen();
    }
    socketserver.close();
  }
}
