import java.io.*;
import java.net.*;

public class server {

  public static int port = 8888;

  public static void main(String[] args) throws IOException {

    InetAddress addr = InetAddress.getLocalHost();
    ServerSocket socketserver = new ServerSocket(port);
    System.out.println(addr);
    System.out.println("Waiting for client connection request...");
    try {
      while (true) {
        Socket client = socketserver.accept();
        try {
          BufferedInputStream in = new BufferedInputStream(client.getInputStream());
          byte[] b = new byte[1024];
          String data = "";
          int length;
          while ((length = in.read(b)) > 0)
          {
            data += new String(b, 0, length);
          }
          System.out.println("Message Received: " + data);
          in.close();
          in = null;
        } catch (IOException e) {
          e.printStackTrace();
        }
        finally {
          break;
        }
      }
    }
    finally {
      socketserver.close();
    }
  }
}
