import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.NetworkInterface;
import java.net.StandardProtocolFamily;
import java.net.StandardSocketOptions;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.util.Date;

public class Main {
  public static void main(String[] args) {
    ByteBuffer datetime;//from www.java2s.com
    try (DatagramChannel datagramChannel = DatagramChannel
        .open(StandardProtocolFamily.INET)) {
      if (datagramChannel.isOpen()) {
        NetworkInterface networkInterface = NetworkInterface.getByName("test");
        datagramChannel.setOption(StandardSocketOptions.IP_MULTICAST_IF,
            networkInterface);
        datagramChannel.setOption(StandardSocketOptions.SO_REUSEADDR, true);
        datagramChannel.bind(new InetSocketAddress(5555));
        while (true) {
          System.out.println("Sending data ...");
          datetime = ByteBuffer.wrap(new Date().toString().getBytes());
          datagramChannel
              .send(datetime,
                  new InetSocketAddress(InetAddress.getByName("225.41.5.16"),
                      5555));
          datetime.flip();
        }
      } else {
        System.out.println("The channel cannot be opened!");
      }
    } catch (IOException ex) {
      System.err.println(ex);
    }
  }
}