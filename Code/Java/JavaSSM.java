// https://docs.oracle.com/javase/8/docs/api/java/net/MulticastSocket.html
// https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/net/MulticastSocket.html
// https://www.baeldung.com/java-broadcast-multicast
// https://medium.com/@AlexanderObregon/introduction-to-multicast-sockets-in-java-c276a897d8e8
// https://www.kompf.de/java/multicast.html
// https://medium.com/@AlexanderObregon/introduction-to-multicast-sockets-in-java-c276a897d8e8

import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class JavaASM {
    public static void main(){

        // port, multicast IP, source IP
        InetAddress multicastAddress = InetAddress.getByName("239.255.255.250");
        final int multicastPort = 1900;
        InetAddress sourceAddress = InetAddress.getByName("250.255.255.250");

        // create socket
        MulticastSocket socket = new MulticastSocket(multicastPort);

        // reuse address
        socket.setReuseAddress(true);

        socket.bind(multicastAddress, multicastPort);

        // Bind to the specified network interface (can be replaced with your interface)
        NetworkInterface networkInterface = NetworkInterface.getByName(INADDR_ANY);

        // join SSM
        socket.joinGroup(multicastAddress, 0, sourceAddress);

        // receive
        byte[] buf = new byte[1024];
        DatagramPacket packet = new DatagramPacket(buf, buf.length);
        while (Thread.currentThread().isInterrupted()) {

            socket.receive(packet);

            String received = new String(packet.getData(), 0, packet.getLength());
            System.out.println(received);
            if ("end".equals(received)) {
                break;
            }
        }
        //clean up
        socket.leaveGroup(multicastAddress);
        socket.close();
    }
}