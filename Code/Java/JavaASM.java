// https://docs.oracle.com/javase/8/docs/api/java/net/MulticastSocket.html
// https://www.baeldung.com/java-broadcast-multicast
// https://medium.com/@AlexanderObregon/introduction-to-multicast-sockets-in-java-c276a897d8e8
// https://www.kompf.de/java/multicast.html
// https://medium.com/@AlexanderObregon/introduction-to-multicast-sockets-in-java-c276a897d8e8


import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class JavaASM { //extends Thread
    public static void main(){
        // port and multicast IP
        InetAddress multicastAddress = InetAddress.getByName("239.255.255.250");
        final int multicastPort = 1900;
        
        // create socket
        MulticastSocket socket = new MulticastSocket(multicastPort);

        // reuse address
        socket.setReuseAddress(true);

        // join ASM
        socket.joinGroup(multicastAddress);

        // receive
        byte[] buf = new byte[1024];
        DatagramPacket packet = new DatagramPacket(buf, buf.length);
        while (Thread.currentThread().isInterrupted()) {

            socket.receive(packet);

            String received = new String(packet.getData(), 0, packet.getLength());
            System.out.println(received);
        }
        //clean up
        socket.leaveGroup(multicastAddress);
        socket.close();
    }
}