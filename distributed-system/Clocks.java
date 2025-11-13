import java.util.*;
import java.time.*;
import java.text.DecimalFormat;

public class Clocks {

    // ---------- NTP (Cristian's Algorithm) ----------
    static void simulateNTP() throws InterruptedException {
        System.out.println("\n=== NTP (Cristian's Algorithm Simulation) ===");
        double networkDelay = 0.05;  // 50ms delay
        double serverOffset = 1.25;  // server ahead by 1.25s

        Instant t1 = Instant.now(); // client send
        Thread.sleep((long)(networkDelay * 1000));

        Instant t2 = Instant.now().plusMillis((long)(serverOffset * 1000)); // server recv
        Thread.sleep(20); // server processing
        Instant t3 = Instant.now().plusMillis((long)(serverOffset * 1000)); // server reply

        Thread.sleep((long)(networkDelay * 1000));
        Instant t4 = Instant.now(); // client recv

        double sec1 = t1.toEpochMilli() / 1000.0;
        double sec2 = t2.toEpochMilli() / 1000.0;
        double sec3 = t3.toEpochMilli() / 1000.0;
        double sec4 = t4.toEpochMilli() / 1000.0;

        double offset = ((sec2 - sec1) + (sec3 - sec4)) / 2.0;
        double delay = (sec4 - sec1) - (sec3 - sec2);

        DecimalFormat df = new DecimalFormat("0.000000");
        System.out.println("Round Trip Delay: " + df.format(delay) + " sec");
        System.out.println("Clock Offset: " + df.format(offset) + " sec");
        System.out.println("=> Client should adjust its clock by " + df.format(offset) + " seconds.");
    }

    // ---------- Lamport Logical Clock ----------
    static class Process {
        int id, clock = 0;
        Process(int i) { id = i; }

        void event() {
            clock++;
            System.out.println("Process " + id + " internal event | LC = " + clock);
        }
        int sendMessage() {
            clock++;
            System.out.println("Process " + id + " sent message | LC = " + clock);
            return clock;
        }
        void receiveMessage(int msgClock) {
            clock = Math.max(clock, msgClock) + 1;
            System.out.println("Process " + id + " received message | LC = " + clock);
        }
    }

    static void simulateLamport() {
        System.out.println("\n=== Lamport Logical Clock Simulation ===");
        Process P1 = new Process(1), P2 = new Process(2);

        P1.event();
        int msg1 = P1.sendMessage();
        P2.receiveMessage(msg1);

        P2.event();
        int msg2 = P2.sendMessage();
        P1.receiveMessage(msg2);

        P1.event();
        P2.event();

        System.out.println("\nFinal Logical Clocks:");
        System.out.println("P1 = " + P1.clock + ", P2 = " + P2.clock);
    }

    // ---------- Main ----------
    public static void main(String[] args) throws Exception {
        Scanner sc = new Scanner(System.in);
        System.out.println("=== CLOCK SYNCHRONIZATION SIMULATION ===");
        System.out.println("1. NTP (Cristian’s Algorithm)\n2. Lamport Logical Clock");
        System.out.print("Enter choice: ");
        int choice = sc.nextInt();

        if (choice == 1) simulateNTP();
        else if (choice == 2) simulateLamport();
        else System.out.println("Invalid choice!");

        System.out.println("\nSimulation Complete ✅");
        sc.close();
    }
}
