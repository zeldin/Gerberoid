package se.pp.mc.android.Gerberoid.utils;

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

        try {
            InputStream inputStream = new BufferedInputStream(context.getContentResolver().openInputStream(uri));
            return copyFile(inputStream, output);
        } catch (Exception e){
            e.printStackTrace();
            return null;
        }

    }

    public static File copyFile(File file, File output){

        try {
            InputStream inputStream = new BufferedInputStream(new FileInputStream(file));
            return copyFile(inputStream, output);
        } catch (Exception e){
            e.printStackTrace();
            return null;
        }

    }

    public static File copyFile(InputStream inputStream, File output){

        try {

            if(!output.getParentFile().exists()){
                output.getParentFile().mkdirs();
            }

            if(!output.exists()){
                output.createNewFile();
            }

            writeToFile(inputStream, output);
            return output;

        } catch (IOException e) {

            e.printStackTrace();
            return null;

        } finally {

            if(inputStream != null){

                try {
                    inputStream.close();
                } catch (IOException e) {}

            }

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

    public static boolean unpackZip(final String path, final String zippath)
    {
        byte[] buffer = new byte[1024];
        int count;

        InputStream is;
        ZipInputStream zis;
        try
        {
            is = new FileInputStream(zippath);
            zis = new ZipInputStream(new BufferedInputStream(is));
            ZipEntry ze;

            while((ze = zis.getNextEntry()) != null)
            {
                final ByteArrayOutputStream baos = new ByteArrayOutputStream();

                if(ze.isDirectory()){ continue; }

                final String filename = ze.getName();
                final File outFile = new File(path + "/" + filename);
                if(!outFile.getParentFile().exists()){
                    outFile.getParentFile().mkdirs();
                }

                final FileOutputStream fout = new FileOutputStream(outFile);

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


    public static File toInternalLayerFile(Context context, Uri uri, int currentLayer) {

        File dataDir = context.getFilesDir();
        File output = new File(dataDir.getAbsolutePath() + "/layers/" + currentLayer + ".layer");
        FileUtils.copyFile(context, uri, output);
        return output;

    }

    public static File toInternalLayerFile(Context context, File file, int currentLayer) {

        File dataDir = context.getFilesDir();
        File output = new File(dataDir.getAbsolutePath() + "/layers/" + currentLayer + ".layer");
        FileUtils.copyFile(file, output);
        return output;

    }

}
