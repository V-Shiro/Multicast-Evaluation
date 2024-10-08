// https://learn.microsoft.com/de-de/dotnet/api/system.net.sockets.udpclient.joinmulticastgroup?view=net-7.0
// https://learn.microsoft.com/en-us/dotnet/api/system.net.sockets.multicastoption?view=net-8.0
// https://www.webdevtutor.net/blog/c-sharp-multicast-socket-example

// https://stackoverflow.com/questions/5145804/how-to-join-source-specific-multicast-group-in-c-sharp-igmpv3
// https://github.com/dotnet/runtime/issues/36170
// https://stackoverflow.com/questions/51422275/c-sharp-socketoptionname-addsourcemembership-unknown-invalid-or-unsupported

using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

public class SourceMulticastOption
{
    public IPAddress Group { get; }
    public IPAddress Source { get; }
    public long InterfaceIndex { get; }
}

class MulticastReceiver
{
    static void Main() {

        IPAddress multicastGroupIP = IPAddress.Parse("239.255.255.250"); //multicast group
        int multicastPort = 1900; //multicast port
        IPAddress sourceIP = IPAddress.Parse("239.255.255.250"); // source IP

        // create UDP socket for multicast
        Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

        // reuse address
        s.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);

        // bind socket
        EndPoint localEP = new IPEndPoint(IPAddress.Any, multicastPort);
        s.Bind(localEP);

        // SSM not officially
        var mcastOption = new SourceMulticastOption(multicastGroupIP, sourceIP, 0);
        s.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.AddSourceMembership, mcastOption);

        // receiving
        byte[] bytes = new Byte[1024];
        EndPoint remoteEP = (EndPoint)new IPEndPoint(IPAddress.Any, 0);
        while (true) {
            // Receive the message.
            int nbytes = s.ReceiveFrom(bytes, ref remoteEP);

                // Convert the byte array to a string.
                string receivedData = Encoding.ASCII.GetString(bytes, 0, nbytes);

                // Display message
                Console.WriteLine(receivedData);
        }
        s.Close();
    }
}