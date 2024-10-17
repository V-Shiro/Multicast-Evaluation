// https://stackoverflow.com/questions/5145804/how-to-join-source-specific-multicast-group-in-c-sharp-igmpv3

using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

class MulticastReceiver {
    static void Main() {
        try
        {
            IPAddress sourceIp = IPAddress.Parse("172.26.159.103");
            Console.WriteLine(sourceIp);
            IPAddress multicastGroupIp = IPAddress.Parse("232.0.0.0");
            //IPAddress multicastGroupIp = IPAddress.Parse("ff05::c)";
            int port = 1900;
            IPAddress localIp = IPAddress.Any;

            Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            IPEndPoint localEndpoint = new IPEndPoint(localIp, port);
            socket.Bind(localEndpoint);

            byte[] membershipAddresses = new byte[12]; // 3 IPs * 4 bytes (IPv4)
            Buffer.BlockCopy(multicastGroupIp.GetAddressBytes(), 0, membershipAddresses, 0, 4);
            Buffer.BlockCopy(sourceIp.GetAddressBytes(), 0, membershipAddresses, 4, 4);
            Buffer.BlockCopy(localIp.GetAddressBytes(), 0, membershipAddresses, 8, 4);
            socket.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.AddSourceMembership, membershipAddresses);

            while (true)
            {
                byte[] buf = new byte[1024];
                int length = socket.Receive(buf);

                Console.WriteLine(Encoding.ASCII.GetString(buf, 0, length));
            }
        }
        catch (Exception e)
        {
            Console.WriteLine(e.Message);
        }
    }
}