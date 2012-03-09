package robocupmonitor;
import java.io.*;

public class LerArquivo {
    public String conteudo;

    public void ler(String url) throws IOException {
        File arquivo = new File(url);
        FileReader leitor = new FileReader(arquivo);

        int c;
        StringBuilder strB = new StringBuilder();
        while((c = leitor.read()) != -1) {
            strB.append((char)c);
        }
        conteudo = new String(strB);
        leitor.close();
    }

    public void gravar(String url) throws IOException {
        File arquivo = new File(url);
        Writer escritor = new OutputStreamWriter(new FileOutputStream(arquivo), "Cp1252");

        escritor.write(conteudo);
        escritor.close();
    }
}
