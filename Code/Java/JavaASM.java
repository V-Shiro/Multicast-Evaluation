// https://docs.oracle.com/javase/8/docs/api/java/net/MulticastSocket.html
// https://www.baeldung.com/java-broadcast-multicast
// https://www.kompf.de/java/multicast.html
// https://medium.com/@AlexanderObregon/introduction-to-multicast-sockets-in-java-c276a897d8e8


import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class Main { 
    public static void main(String[] args){

        // port and multicast IP
        InetAddress multicastAddress = null;
        final int multicastPort = 1900;

        MulticastSocket socket = null;
        DatagramPacket packet = null; 
        byte[] buf = null;   

        try { 
            // set multicast ip
            multicastAddress = InetAddress.getByName("232.0.0.0");

            // create socket
            socket = new MulticastSocket(multicastPort);
 
            // reuse address
            socket.setReuseAddress(true);

            // join ASM
            socket.joinGroup(multicastAddress);

            // receive
            buf = new byte[1024];
            packet = new DatagramPacket(buf, buf.length);
            while (Thread.currentThread().isInterrupted()) {
                socket.receive(packet);
                String received = new String(packet.getData(), 0, packet.getLength());
                System.out.println(received);
            }
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }  
        try { //clean up
            socket.leaveGroup(multicastAddress);
            socket.close();
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }  
    }
}