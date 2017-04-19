package pratice.getcontacts;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.database.Cursor;
import android.provider.ContactsContract.CommonDataKinds.Phone;

public class GetNumber {
	public static List<PhoneInfo> list = new ArrayList<PhoneInfo>();
	
	public static String getNumber(Context	context) {
		Cursor cursor = context.getContentResolver().query(Phone.CONTENT_URI, null, null, null, null);
		String phoneNumber;
		String phoneName;
		while (cursor.moveToNext()) {
			phoneNumber = cursor.getString(cursor.getColumnIndex(Phone.NUMBER));
			phoneName   = cursor.getString(cursor.getColumnIndex(Phone.DISPLAY_NAME));
			PhoneInfo info = new PhoneInfo(phoneName, phoneNumber);
			list.add(info);
		}
		return null;
	}
}
