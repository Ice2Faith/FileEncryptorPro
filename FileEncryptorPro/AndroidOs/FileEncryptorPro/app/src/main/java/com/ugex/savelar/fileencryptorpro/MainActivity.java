package com.ugex.savelar.fileencryptorpro;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;

import android.Manifest;
import android.app.Activity;
import android.content.DialogInterface;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;

public class MainActivity extends Activity {
    private EditText edtSrcFileName;
    private EditText edtSaveDir;
    private EditText edtPassword;
    private RadioGroup rgpType;
    private Button btnBegin;
    private TextView tvLogInfo;
    private CheckBox ckbDelSrcFile;

    private class ProcessInfo
    {
        public boolean state=false;
        public String curFileStr="";
        public String curSaveStr="";
        public int sumCount=0;
        public int processCount=0;
        public int successCount=0;
    }
    private static final int WHAT_THREAD_INFO =0x01;
    private static final int WHAT_NOT_EXIST_FILE=0x02;
    private static final int WHAT_PROCESS_BEGIN=0x03;
    private static final int WHAT_PROCESS_END=0x04;
    private Handler handler=new Handler()
    {
        @Override
        public void handleMessage(@NonNull Message msg) {
            if(msg.what==WHAT_THREAD_INFO)
            {
                ProcessInfo info=(ProcessInfo)msg.obj;
                String logstr="处理中：\n"
                        +"源文件："+info.curFileStr+"\n"
                        +"新文件"+info.curSaveStr+"\n"
                        +"结果："+info.state+"\n"
                        +"当前：成功：总共："+info.processCount+"/"+info.successCount+"/"+info.sumCount+"\n"
                        +"成功率："+(info.successCount*1.0/info.sumCount)+"\n"
                        +"失败率："+(1.0-info.successCount*1.0/info.sumCount);
                tvLogInfo.setText(logstr);
            }else if(msg.what==WHAT_PROCESS_BEGIN)
            {
                tvLogInfo.setText("任务开始处理...");
            }
            else if(msg.what==WHAT_PROCESS_END)
            {
                tvLogInfo.setText("任务处理结束！");
            }
            else if(msg.what==WHAT_NOT_EXIST_FILE)
            {
                tvLogInfo.setText("源文件或文件夹不存在！");
            }
            super.handleMessage(msg);
        }
    };

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("usefl-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

       InitActivity();
    }

    private void InitActivity() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            requestPermissions(new String[]{
                    Manifest.permission.READ_EXTERNAL_STORAGE,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE
            },0x01);
        }

        edtSrcFileName=findViewById(R.id.editTextSrcFileName);
        edtSaveDir =findViewById(R.id.editTextSaveToDir);
        edtPassword=findViewById(R.id.editTextPassword);
        rgpType=findViewById(R.id.radioGroupType);
        btnBegin=findViewById(R.id.buttonBegin);
        tvLogInfo=findViewById(R.id.textViewLog);
        ckbDelSrcFile=findViewById(R.id.checkBoxDelSrcFile);

        edtSaveDir.setText("FileEncryptorPro");

    }

//本地方法
//    public native String stringFromJNI();

    public native boolean fileLock(String srcFileName,String drtFileName,String password);

    public native boolean fileUnlock(String srcFileName,String drtFileName,String password);

    private String g_srcFileName="";
    private String g_saveFileName="";
    private String g_password="";
    private boolean g_isLockMode=true;
    private boolean g_needDeleteSrc=false;
    public void OnBtnBeginClicked(View view) {
        g_srcFileName=edtSrcFileName.getText().toString().trim();
        g_saveFileName= edtSaveDir.getText().toString().trim();
        g_password=edtPassword.getText().toString().trim();

        if(g_srcFileName.length()==0 || g_saveFileName.length()==0 || g_password.length()==0)
        {
            Toast.makeText(this, "请填写完整参数后重试", Toast.LENGTH_SHORT).show();
            return;
        }


        if(g_srcFileName.charAt(0)!='/')
        {
            g_srcFileName=new File(Environment.getExternalStorageDirectory(),g_srcFileName).getAbsolutePath();
        }

        if(g_saveFileName.charAt(0)!='/')
        {
            g_saveFileName=new File(Environment.getExternalStorageDirectory(),g_saveFileName).getAbsolutePath();
        }

        g_isLockMode=rgpType.getCheckedRadioButtonId()==R.id.radioButtonLock;

        g_needDeleteSrc=ckbDelSrcFile.isChecked();

        edtSrcFileName.setText(g_srcFileName);
        edtSaveDir.setText(g_saveFileName);
        edtPassword.setText(g_password);

        AlertDialog dlg=new AlertDialog.Builder(this)
                .setIcon(R.drawable.ic_launcher)
                .setTitle("操作确认请求")
                .setMessage("即将开始操作，是否继续？\n误操作请点击【取消】")
                .setNegativeButton("继续", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        Toast.makeText(MainActivity.this, "任务已开始，请耐心等待...", Toast.LENGTH_SHORT).show();
                        threadProcess(g_srcFileName,g_saveFileName,g_password,g_isLockMode,g_needDeleteSrc);
                    }
                })
                .setPositiveButton("取消", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        Toast.makeText(MainActivity.this, "任务已取消!", Toast.LENGTH_SHORT).show();
                    }
                })
                .create();
        dlg.show();

    }

    private class RunnableClassThread implements Runnable
    {
        private String srcFile;
        private String saveDir;
        private String password;
        private boolean isLockMode;
        private boolean needDelSrc;

        private boolean state=false;
        private String curFileStr="";
        private String curSaveStr="";
        private int sumCount=0;
        private int processCount=0;
        private int successCount=0;
        public RunnableClassThread(String srcFile, String saveDir, String password, boolean isLockMode,boolean needDelSrc) {
            this.srcFile = srcFile;
            this.saveDir = saveDir;
            this.password = password;
            this.isLockMode = isLockMode;
            this.needDelSrc=needDelSrc;
        }
        private void processPath(File sfile,File save)
        {
            if(sfile.exists()==false)
                return;

            if(sfile.isFile()) {

                File newFile=null;
                boolean isSuccess=false;
                if(isLockMode)
                {
                    newFile=new File(save,sfile.getName()+".flcl");
                    isSuccess=fileLock(sfile.getAbsolutePath(),newFile.getAbsolutePath(),password);
                }
                else
                {
                    String sname=sfile.getName();
                    if(sname.endsWith(".flcl"))
                    {
                        sname=sname.substring(0,sname.lastIndexOf(".flcl"));
                    }
                    newFile=new File(save,sname);
                    isSuccess=fileUnlock(sfile.getAbsolutePath(),newFile.getAbsolutePath(),password);
                }

                state=isSuccess;
                curFileStr=sfile.getAbsolutePath();
                curSaveStr=newFile.getAbsolutePath();
                sumCount++;
                processCount++;
                if(state){
                    successCount++;
                    if(needDelSrc)
                    {
                       sfile.delete();
                    }
                }




                Message msg=new Message();
                msg.what=WHAT_THREAD_INFO;
                ProcessInfo info=new ProcessInfo();
                info.curFileStr=curFileStr;
                info.curSaveStr=curSaveStr;
                info.processCount=processCount;
                info.state=state;
                info.successCount=successCount;
                info.sumCount=sumCount;
                msg.obj=info;
                handler.sendMessage(msg);

            }else if(sfile.isDirectory()){
                File curSaveDir=new File(save,sfile.getName());
                curSaveDir.mkdirs();

                for(File fl : sfile.listFiles())
                {
                    processPath(fl,curSaveDir);
                }
            }
        }
        @Override
        public void run() {
            File sfile=new File(srcFile);
            if(sfile.exists()==false)
            {
                Message msg=new Message();
                msg.what=WHAT_NOT_EXIST_FILE;
                msg.obj=srcFile;
                handler.sendMessage(msg);
                return;
            }

            Message msgb=new Message();
            msgb.what=WHAT_PROCESS_BEGIN;
            handler.sendMessage(msgb);

            processPath(sfile,new File(saveDir));

            Message msge=new Message();
            msge.what=WHAT_PROCESS_END;
            handler.sendMessage(msge);
        }
    }
    private void threadProcess(String srcFile,String saveDir,String password,boolean isLock,boolean needDelSrc)
    {
        RunnableClassThread threadRun=new RunnableClassThread(srcFile,saveDir,password,isLock,needDelSrc);
        new Thread(threadRun).start();
    }

    public void OnTvTipsClicked(View view) {
        AlertDialog dlg=new AlertDialog.Builder(this)
                .setIcon(R.mipmap.ic_launcher)
                .setTitle("使用帮助")
                .setMessage("请填写操作文件的完整路径后，点击开始按钮，即可进行相应的操作\n" +
                        "源文件可以是文件，也可以是文件夹\n" +
                        "保存到目录可以不存在，运行开始时自动创建\n" +
                        "密码可以是任意的字符串，字母数字符号，甚至是中文都可以\n" +
                        "源文件和保存路径如果不是绝对路径，那么将默认相对外部存储根目录\n" +
                        "\t也就是：/sdcard/\n" +
                        "删除源文件，只会删除成功操作的项，不会删除操作失败的项\n" +
                        "\t这点你可以放心")
                .setPositiveButton("好的",null)
                .create();
        dlg.show();
    }
}
