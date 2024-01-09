
import java.io.*;
import java.nio.file.*;

public class copyFile
{

    public static void main ( String[] args )
    {
        if (args.length < 2)
	{
            System.out.println("Usage: <input> <output file>\n") ;
            System.exit(0) ;
        }
 
        String inFileName  = args[0];
        String outFileName = args[1];
 
        try
	{
            long start = System.currentTimeMillis();

            byte[] allBytes = Files.readAllBytes(Paths.get(inFileName));
            Files.write(Paths.get(outFileName), allBytes);
 
            long end = System.currentTimeMillis();
 
            System.out.println("Time: " + (end - start) + " ms;");
        }
	catch (IOException ex)
	{
            ex.printStackTrace();
        }
    }

}

