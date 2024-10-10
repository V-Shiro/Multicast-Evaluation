// https://docs.oracle.com/javase/8/docs/api/java/net/MulticastSocket.html
// https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/net/MulticastSocket.html
// https://www.baeldung.com/java-broadcast-multicast
// https://www.kompf.de/java/multicast.html
// https://medium.com/@AlexanderObregon/introduction-to-multicast-sockets-in-java-c276a897d8e8


// not supported

import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class Main {
    public static void main(){

        // port, multicast IP, source IP
        InetAddress multicastAddress = null;
        final int multicastPort = 1900;
        InetAddress sourceAddress = null;
        
        MulticastSocket socket = null;
        DatagramPacket packet = null; 
        byte[] buf = null;  

        try { 
            // set multicast ip and source ip
            multicastAddress = InetAddress.getByName("232.0.0.0");
            sourceAddress = InetAddress.getByName("250.255.255.250");

            // create socket
            socket = new MulticastSocket(multicastPort);

            // reuse address
            socket.setReuseAddress(true);

            // join SSM
            socket.joinGroup(multicastAddress, 0, sourceAddress);

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
        try {//clean up
            socket.leaveGroup(multicastAddress);
            socket.close();
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }  
    }
}