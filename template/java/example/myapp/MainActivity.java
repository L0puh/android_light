package example.myapp;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import android.widget.TextView;
public class MainActivity extends Activity {

   static {
      System.loadLibrary("native-lib");
   }

    private TextView text;
    public native String helloworld();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        text = (TextView) findViewById(R.id.my_text);
        text.setText(helloworld());

        Button button = (Button) findViewById(R.id.my_btn);
        button.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
              Toast.makeText(MainActivity.this, "Button clicked!", Toast.LENGTH_SHORT).show();
           }
        });
    }
}
