package pratice.getcontacts;

import android.app.Activity;
import android.os.Bundle;
import android.widget.ListView;


public class MainActivity extends Activity {
	private ListView showView;
	private MyAdapter adapter;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        GetNumber.getNumber(this);
        showView = (ListView) findViewById(R.id.showContacts);
        adapter = new MyAdapter(GetNumber.list, this);
        //°ó¶¨
        showView.setAdapter(adapter);
        
    }

}
