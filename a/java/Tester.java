import zeidon.zView;
import java.io.*;

public class Tester
{
    /**
     * @param args
     */
    public static void main(String[] args) throws Exception
    {
        System.out.println("java.library.path = " + System.getProperty("java.library.path"));
//        System.load("/home/dchristensen/zeidon/10d/x/bin/kzoejava.so");
        Process p = Runtime.getRuntime().exec("sh ldd /home/dchristensen/zeidon/10d/x/bin/libkzoejava.so");
//        if(p.waitFor() != 0)
            p.waitFor();
            System.out.println(getOutAndErrStream(p));
        System.loadLibrary("kzoejava");
        zView view = new zView("abc");
    }

    private static String getOutAndErrStream(Process p){

        StringBuffer cmd_out = new StringBuffer("");
        if(p != null){
            BufferedReader is = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String buf = "";
            try{
                while((buf = is.readLine()) != null){
                    cmd_out.append(buf);
                    cmd_out.append (System.getProperty("line.separator"));
                }
                is.close();
                is = new BufferedReader(new InputStreamReader(p.getErrorStream()));
                while((buf = is.readLine()) != null){
                    cmd_out.append(buf);
                    cmd_out.append("\n");
                }
                is.close();
            }catch(Exception e){
                e.printStackTrace();
            }
        }
        return cmd_out.toString();
    } 
}
