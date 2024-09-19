// Quelle: https://learn.microsoft.com/de-de/dotnet/api/system.net.sockets.udpclient.joinmulticastgroup?view=net-7.0
// https://learn.microsoft.com/en-us/dotnet/api/system.net.sockets.multicastoption?view=net-8.0
// https://www.webdevtutor.net/blog/c-sharp-multicast-socket-example

// https://stackoverflow.com/questions/5145804/how-to-join-source-specific-multicast-group-in-c-sharp-igmpv3

using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

class MulticastReceiver
{
    public static void Main(string[] args)
    {
        if (args.Length != 4)
            Console.Writeline("Command line args should be multicast group, port and source IP.");
            Console.Writeline("(e.g. for SSDP, `listener 239.255.255.250 1900 239.255.255.250`)");

        IPAddress multicastGroupIP = IPAdress.Parse(args[1]); //multicast group
        int multicastPort = args[2]; //multicast port
        IPAddress sourceIP = IPAdress.Parse(args[3]); // source IP

        try {
            // Create a UdpClient/socket to listen for multicast messages.
            //UdpClient socket = new UdpClient(multicastPort);
            s = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            //EndPoint localEP = (EndPoint)new IPEndPoint(IPAddress.Any, multicastPort);
            IPEndPoint localEP = new IPEndPoint(IPAddress.Any, multicastPort);

            s.Bind(localEP);

            sOptions = new MulticastOption(multicastGroupIP, IPAddress.Any)

            // SSM or ASM
            s.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.AddMembership, sOptions)
            //s.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.AddSourceMembership, sourceIP)

        }
        catch (Exception e){
            Console.WriteLine(e.ToString());
        }

        // Join the multicast group. SERVER SITE?
        //s.JoinMulticastGroup(IPAddress.Parse(multicastGroupIP));

        // Data storage
        IPEndPoint remoteEndPoint = new IPEndPoint(IPAddress.Any, multicastPort);

        // receiving
        while (true) {
            // Receive the message.
            byte[] receiveBytes = s.Receive(ref remoteEndPoint);

            // Convert the byte array to a string.
            string receivedData = Encoding.ASCII.GetString(receiveBytes);

            // Display message
            //Console.WriteLine($"Received message from {remoteEndPoint}: {receivedData}");
            Console.WriteLine(receivedData);
        }
        s.Close();
    }
}