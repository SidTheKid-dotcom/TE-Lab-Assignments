import java.net.*;
import java.io.*;

public class Server {
    public static void main(String[] args) throws Exception {
        ServerSocket ss = new ServerSocket(9090);
        System.out.println("Server on 9090...");
        while (true) {
            Socket s = ss.accept();
            new Thread(() -> {
                try (s; var in = new BufferedReader(new InputStreamReader(s.getInputStream()));
                     var out = new PrintWriter(s.getOutputStream(), true)) {
                    String line;
                    while ((line = in.readLine()) != null) {
                        System.out.println("Got: " + line);
                        out.println(line); // echo back
                    }
                } catch (Exception e) { }
            }).start();
        }
    }
}
