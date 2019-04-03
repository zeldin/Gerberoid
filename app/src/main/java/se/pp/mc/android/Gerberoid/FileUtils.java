package se.pp.mc.android.Gerberoid;

import android.content.Context;
import android.net.Uri;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class FileUtils {

    public static File writeUriToTempFile(Context context, Uri uri, String prefix){

        try {
            File tmp = File.createTempFile(prefix, null);
            return copyFile(context, uri, tmp);
        } catch (Exception e){
            return null;
        }

    }

    public static File copyFile(Context context, Uri uri, File output){

        try (InputStream inputStream = new BufferedInputStream(context.getContentResolver().openInputStream(uri))) {

            if(!output.getParentFile().exists()){
                output.getParentFile().mkdirs();
            }

            if(!output.exists()){
                output.createNewFile();
            }

            writeToFile(inputStream, output);
            return output;

        } catch (IOException e) {
            return null;
        }

    }

    public static File copyFile(File file, File output){

        try (InputStream inputStream = new BufferedInputStream(new FileInputStream(file))) {

            if(!output.getParentFile().exists()){
                output.getParentFile().mkdirs();
            }

            if(!output.exists()){
                output.createNewFile();
            }

            writeToFile(inputStream, output);
            return output;

        } catch (IOException e) {
            return null;
        }

    }

    public static void writeToFile(InputStream is, File file) throws IOException {

        OutputStream outputStream = null;

        try {
            outputStream = new FileOutputStream(file);

            int read = 0;
            byte[] bytes = new byte[1024];
            while ((read = is.read(bytes)) != -1) {
                outputStream.write(bytes, 0, read);
            }
        } finally {
            if (outputStream != null) {
                outputStream.close();
            }
        }
    }

    public static boolean unpackZip(String path, String zippath)
    {
        InputStream is;
        ZipInputStream zis;
        try
        {
            is = new FileInputStream(zippath);
            zis = new ZipInputStream(new BufferedInputStream(is));
            ZipEntry ze;

            while((ze = zis.getNextEntry()) != null)
            {
                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                byte[] buffer = new byte[1024];
                int count;

                String filename = ze.getName();
                FileOutputStream fout = new FileOutputStream(path + "/" + filename);

                // reading and writing
                while((count = zis.read(buffer)) != -1)
                {
                    baos.write(buffer, 0, count);
                    byte[] bytes = baos.toByteArray();
                    fout.write(bytes);
                    baos.reset();
                }

                fout.close();
                zis.closeEntry();
            }

            zis.close();
        }
        catch(IOException e)
        {
            e.printStackTrace();
            return false;
        }

        return true;
    }

}
