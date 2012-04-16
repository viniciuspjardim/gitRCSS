package robocupmonitor;

import com.sun.java.swing.plaf.gtk.GTKLookAndFeel;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import javax.swing.UIManager;

/**
 *
 * @author vinicius
 */
public class Formulario extends javax.swing.JFrame {

    // Marca se a interface deve ser atualizada
    private volatile boolean rodando = false;
    // Classe (runnable) que atualiza a interface
    private UpdateGui update = new UpdateGui();
    // tempo em milisegundos que a interface deve ser atualizada
    private int tempoAtualizacao;
    
    // Caminhos dos arquivos necessários
    private String urlScript = "";
    private String urlTime1 = "";
    private String urlTime2 = "";
    
    public Formulario(String[] args) {
        initComponents();
        try {
            urlScript = args[0];
            urlTime1 = args[1];
            urlTime2 = args[2];
        } catch (Exception e) {
            terminalTArea.setText("**Erro: argumentos invalidos**");
        }
        
        try {
            tempoAtualizacao = Integer.parseInt(args[3]);
        } catch (Exception e) {
            tempoAtualizacao = 500;
        }
        terminalTArea.setText("Tempo de atualização da interface: "
                + tempoAtualizacao + "ms");
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jLabel1 = new javax.swing.JLabel();
        iniciarButton = new javax.swing.JButton();
        pararButton = new javax.swing.JButton();
        jTabbedPane1 = new javax.swing.JTabbedPane();
        jPanel1 = new javax.swing.JPanel();
        scroolTerminal = new javax.swing.JScrollPane();
        terminalTArea = new javax.swing.JTextArea();
        jPanel2 = new javax.swing.JPanel();
        jScrollPane2 = new javax.swing.JScrollPane();
        time1TArea = new javax.swing.JTextArea();
        jPanel3 = new javax.swing.JPanel();
        jScrollPane1 = new javax.swing.JScrollPane();
        time2TArea = new javax.swing.JTextArea();
        statusLabel = new javax.swing.JLabel();
        jCheckBox1 = new javax.swing.JCheckBox();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Info");

        jLabel1.setText("Jogo:");

        iniciarButton.setText("iniciar");
        iniciarButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                iniciarButtonActionPerformed(evt);
            }
        });

        pararButton.setText("parar");
        pararButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                pararButtonActionPerformed(evt);
            }
        });

        terminalTArea.setColumns(20);
        terminalTArea.setRows(5);
        scroolTerminal.setViewportView(terminalTArea);

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(scroolTerminal, javax.swing.GroupLayout.DEFAULT_SIZE, 401, Short.MAX_VALUE)
                .addContainerGap())
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(scroolTerminal, javax.swing.GroupLayout.DEFAULT_SIZE, 319, Short.MAX_VALUE)
                .addContainerGap())
        );

        jTabbedPane1.addTab("Terminal", jPanel1);

        time1TArea.setColumns(20);
        time1TArea.setRows(5);
        time1TArea.setText("Jogador\t|B|a-o|E|\n\nB = nosso time está com a bola? V = está, F = não está;\na = número aliados no centro de jogo;\no = número oponentes no centro de jogo;\nE = estado do jogo. A ataque, D defesa, CA contra-ataque, CD");
        jScrollPane2.setViewportView(time1TArea);

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 401, Short.MAX_VALUE)
                .addContainerGap())
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 319, Short.MAX_VALUE)
                .addContainerGap())
        );

        jTabbedPane1.addTab("Time 1", jPanel2);

        time2TArea.setColumns(20);
        time2TArea.setRows(5);
        time2TArea.setText("Jogador\tEstado");
        jScrollPane1.setViewportView(time2TArea);

        javax.swing.GroupLayout jPanel3Layout = new javax.swing.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 401, Short.MAX_VALUE)
                .addContainerGap())
        );
        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 319, Short.MAX_VALUE)
                .addContainerGap())
        );

        jTabbedPane1.addTab("Time 2", jPanel3);

        statusLabel.setText("Parado");

        jCheckBox1.setText("Repetir");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel1)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(iniciarButton)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(pararButton)
                .addGap(18, 18, 18)
                .addComponent(statusLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(jCheckBox1)
                .addContainerGap())
            .addComponent(jTabbedPane1)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel1)
                    .addComponent(iniciarButton)
                    .addComponent(pararButton)
                    .addComponent(statusLabel)
                    .addComponent(jCheckBox1))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jTabbedPane1))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void iniciarButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_iniciarButtonActionPerformed
        rodando = true;
        new Thread(update).start();
    }//GEN-LAST:event_iniciarButtonActionPerformed

    private void pararButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_pararButtonActionPerformed
        rodando = false;
        executarScripts(new ProcessBuilder("killall", "rcssserver"));
        executarScripts(new ProcessBuilder("killall", "rcssmonitor"));
        
    }//GEN-LAST:event_pararButtonActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(final String[] args) {

        try {
            UIManager.setLookAndFeel(new GTKLookAndFeel());
        } catch (Exception e) {e.printStackTrace();}
        //</editor-fold>

        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {
                new Formulario(args).setVisible(true);
            }
        });
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton iniciarButton;
    private javax.swing.JCheckBox jCheckBox1;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JTabbedPane jTabbedPane1;
    private javax.swing.JButton pararButton;
    private javax.swing.JScrollPane scroolTerminal;
    private javax.swing.JLabel statusLabel;
    private javax.swing.JTextArea terminalTArea;
    private javax.swing.JTextArea time1TArea;
    private javax.swing.JTextArea time2TArea;
    // End of variables declaration//GEN-END:variables
    
    private String lerTime(String urlTime) {
        StringBuilder strBTime = new StringBuilder("Jogador\t|B|a-o|E|\n\n");
        LerArquivo leitor = new LerArquivo();
        try {
            // Ler o arquvio do tecnico
            leitor.ler(urlTime + "coach.txt");
            strBTime.append(leitor.conteudo);
            
            // Ler arquivos dos jogadores
            for (int i = 1; i < 12; i++) {
                leitor.ler(urlTime + i +".txt");
                strBTime.append(leitor.conteudo);
            }
        } catch (IOException e) {
            e.printStackTrace();
            strBTime.append("\n**Falha ao ler arquivos no diretório '"
                    + urlTime + "'!**");
            if(urlTime.equals(urlTime1))
                urlTime1 = null;
            else if(urlTime.equals(urlTime2))
                urlTime2 = null;
        }
        strBTime.append("\nB = nosso time está com a bola? V = está, F = não está;\na = número aliados no centro de jogo;\no = número oponentes no centro de jogo;\nE = estado do jogo. A ataque, D defesa,\nCA contra-ataque, CD contra-defesa.");
        return new String(strBTime);
    }
    
    public Process executarScripts(ProcessBuilder pBuilder) {
        try {
            Process proc = pBuilder.start();
            return proc;
        } catch (Exception ex) {
            ex.printStackTrace();
            terminalTArea.setText("**Falha ao tentar executar script!**");
        }
        return null;
    }
    
    private class UpdateGui implements Runnable{
        
        @Override
        public void run() {
            
            Process proc = executarScripts(new ProcessBuilder(urlScript));
            if(proc == null) {
                pararButtonActionPerformed(null);
                return;
            }
            
            statusLabel.setText("Rodando");
            LerSaida ler = new LerSaida(proc);
            new Thread(ler).start();
            
            while (rodando) {
                // Atualiza os dados lidos dos times
                if(statusLabel.getText().length() < 10) {
                    statusLabel.setText(statusLabel.getText() + ".");
                } else {
                    statusLabel.setText("Rodando");
                }
                if(urlTime1 != null)
                    time1TArea.setText(lerTime(urlTime1));
                if(urlTime2 != null)
                    time2TArea.setText(lerTime(urlTime2));
                
                // Atualiza o terminal
                terminalTArea.append(ler.novoConteudo());
                
                // Pausa a atualização da interface por 500 milissegundos
                try {
                    Thread.sleep(tempoAtualizacao);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                terminalTArea.setCaretPosition(terminalTArea.getDocument().getLength());
            }
            statusLabel.setText("Parado");
        }
    }
    
    private class LerSaida implements Runnable {
        private volatile Process p;
        StringBuffer strB = new StringBuffer();
        
        public LerSaida(Process pArg) {
            p = pArg;
        }
        
        public String novoConteudo() {
            String str = new String(strB);
            strB.delete(0, strB.length());
            
            return str;
        }
        
        @Override
        public void run() {
            InputStream is = p.getInputStream();
            InputStreamReader isr = new InputStreamReader(is);
            BufferedReader br = new BufferedReader(isr);
            String s;
            while (p != null) {
                try {
                    while ((s = br.readLine()) != null) {
                        strB.append(s + "\n");
                    }
                } catch (Exception ex) {
                    ex.printStackTrace();
                    strB.append("\n**Falha ao ler saída do terminal!**");
                    p = null;
                }
                try {
                    Thread.sleep(tempoAtualizacao);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
