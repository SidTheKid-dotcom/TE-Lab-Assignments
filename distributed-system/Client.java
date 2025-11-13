import java.net.*;
import java.io.*;

public class Client {
    public static void main(String[] args) throws Exception {
        Socket s = new Socket("127.0.0.1", 9090);
        var in = new BufferedReader(new InputStreamReader(s.getInputStream()));
        var out = new PrintWriter(s.getOutputStream(), true);
        var console = new BufferedReader(new InputStreamReader(System.in));
        System.out.println("Connected. Type messages (exit to quit):");
        String msg;
        while ((msg = console.readLine()) != null) {
            if (msg.equals("exit")) break;
            out.println(msg);
            System.out.println("Echo: " + in.readLine());
        }
        s.close();
    }
}
