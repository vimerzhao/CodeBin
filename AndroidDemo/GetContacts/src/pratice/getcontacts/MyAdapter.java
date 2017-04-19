package pratice.getcontacts;

import java.util.List;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.LinearLayout;
import android.widget.TextView;

public class MyAdapter extends BaseAdapter {
	private List<PhoneInfo> lists;
	private Context	context;
	private LinearLayout layout;
	public MyAdapter(List<PhoneInfo> lists, Context	context) {
		this.lists = lists;
		this.context = context;
	}
	
	@Override
	public int getCount() {
		return lists.size();
	}

	@Override
	public Object getItem(int arg0) {
		return lists.get(arg0);
	}

	@Override
	public long getItemId(int arg0) {
		return arg0;
	}

	@Override
	public View getView(int arg0, View arg1, ViewGroup arg2) {
		//权限
//		LayoutInflater inflater = LayoutInflater.from(context);
//		layout = (LinearLayout) inflater.inflate(R.layout.cell, null);
//		TextView nameView = (TextView) layout.findViewById(R.id.name);
//		TextView numberView = (TextView) layout.findViewById(R.id.number);
//		nameView.setText(lists.get(arg0).getName());
//		numberView.setText(lists.get(arg0).getNumber());
//		return layout;
		
		//优化
		ViewHolder holder;
		
		if (arg1 == null) {
			arg1 = LayoutInflater.from(context).inflate(R.layout.cell, null);
			holder = new ViewHolder();
			holder.nameTV = (TextView) arg1.findViewById(R.id.name);
			holder.numberTV = (TextView) arg1.findViewById(R.id.number);
			holder.nameTV.setText(lists.get(arg0).getName());
			holder.numberTV.setText(lists.get(arg0).getNumber());
			arg1.setTag(holder);
		} else {
			holder = (ViewHolder) arg1.getTag();
			holder.nameTV.setText(lists.get(arg0).getName());
			holder.numberTV.setText(lists.get(arg0).getNumber());
		}
		return arg1;
	}
	private static class ViewHolder {
		TextView nameTV;
		TextView numberTV;
	}

}
