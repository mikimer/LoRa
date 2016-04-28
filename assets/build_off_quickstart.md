# Build off of the LoRa-Arduino quickstart project

Here are tips, tricks, and other resources:

* The Arduino sketch only tries to join the LoRa network in the beginning of the program, not continuously -- it issues the AT+JOIN command in setup(), not in loop().  

* If you change the code, there is a huge difference between the print() and println() commands. The Arduino issues a carriage return (`Enter`) after a println(), but not after a print().   

* [Include a Particle Spark version of the code from Andrew @ Sutro]


* Here are Google Script functions that we used to copy/paste formulas 

```java 
// function to add formulas to decode the JSON payload 
function copy_paste_formulas(){

  var ss = SpreadsheetApp.getActiveSpreadsheet();
  var daily_sheet = ss.getSheets()[0];
 
  var rangeToCopy = daily_sheet.getRange(1, 3, 1, daily_sheet.getLastColumn());   // copy the formulas from the 1st row
  rangeToCopy.copyTo(daily_sheet.getRange(3,3,daily_sheet.getLastRow()-2, daily_sheet.getLastColumn()) );  // and paste everywhere below headers on row 2
   
} //end copy_paste_formulas()



// function to store data in a new sheet
function store_yesterdays_data() {
  var ss = SpreadsheetApp.getActiveSpreadsheet();  // get the current spreadsheet
  var daily_sheet = ss.getSheets()[0];             // get the first sheet where Senet/Zapier dumps data
  var lora_sheet = ss.getSheets()[1];              // get the sheet where we show daily values to reflect usage of LoRa
  var storage_sheet = ss.insertSheet(3);           // create a new sheet to store the old data 
 
  // name the storage_sheet with yesterday's date
  var yesterday = new Date();                      // create a new variable 'yesterday' at first with today's date
  yesterday.setDate(yesterday.getDate() - 1);      // now put yesterday's date in 'yesterday'  
  var formattedDate = Utilities.formatDate(yesterday, "GMT", "EEE MMM dd");  // get the date in a clean format 
  storage_sheet.setName('' + formattedDate );      // name the storage sheet with the date
  var formattedTime = Utilities.formatDate(yesterday, "PST", "h:mm a");
  
  // copy headers from daily_sheet to storage_sheet
  var rangeToCopy = daily_sheet.getRange(2, 1, 1, daily_sheet.getLastColumn() );  // row1 = stored formulas; row2 = headers 
  rangeToCopy.copyTo(storage_sheet.getRange(1, 1));
  // copy data from daily_sheet to storage_sheet
  rangeToCopy = daily_sheet.getRange(3, 1, daily_sheet.getLastRow(), daily_sheet.getLastColumn());
  rangeToCopy.copyTo(storage_sheet.getRange(2, 1), {contentsOnly:true});  // On Fri Mar 25th, Mike fixed a formula error by copy-pasting values only 
    
  // calculate the usage of LoRa data
  var payloads_per_day = storage_sheet.getLastRow() - 1;  // every row is a payload of data, except the header 
  var data_range = storage_sheet.getRange(1, 1, storage_sheet.getLastRow(), storage_sheet.getLastColumn() ).getValues();
  var blank_count = 0;
  var byte_count = 0;  
  var temp = "";
  var max_rows = storage_sheet.getLastRow();
  var max_cols = storage_sheet.getLastColumn();
  // use a for-loop to examine all cells for bytes
  for (var i = 2; i < max_rows; i++) {  // review cells in all rows, except the header row  
    for (var j = 8; j < max_cols+1; j++) { // review cells in all columns starting at column i
      temp = storage_sheet.getRange(i,j,1,1).getValue();
      if (temp == "" || temp == " "){ blank_count++; }
      else byte_count++;
    }
  }  
  var avg_payload = byte_count/payloads_per_day;  
   
  // put the data in the lora_sheet
  lora_sheet.getRange(lora_sheet.getLastRow()+1 ,1).setValue(formattedDate);
  lora_sheet.getRange(lora_sheet.getLastRow(), 2).setValue(storage_sheet.getLastRow());
  lora_sheet.getRange(lora_sheet.getLastRow(), 3).setValue(avg_payload).setNumberFormat("0.0");  
  lora_sheet.getRange(lora_sheet.getLastRow() ,4).setValue(formattedTime);
  

  // delete all the old content from the daily sheet, starting at row 3. row 1 = headers; row 2 = formulas.
  var range_to_clear = daily_sheet.getRange(3, 1, daily_sheet.getLastRow(), daily_sheet.getLastColumn()); 
  range_to_clear.clear(); 
  
  // hide the old data sheet to save space to keep things clean
  storage_sheet.hideSheet();
      
} // end store_yesterdays_data() 


/* Links I used:

formatDate()
https://developers.google.com/apps-script/reference/utilities/utilities#formatdatedate-timezone-format
http://docs.oracle.com/javase/7/docs/api/java/text/SimpleDateFormat.html

setName()
https://developers.google.com/apps-script/reference/spreadsheet/sheet#setName(String)

onEdit() and Date()
https://developers.google.com/apps-script/guides/triggers/

getSheetId()
https://developers.google.com/apps-script/reference/spreadsheet/sheet#getSheetId()

yesterday
http://stackoverflow.com/questions/5511323/javascript-yesterday

copyTo()
https://developers.google.com/apps-script/reference/spreadsheet/range#copytodestination

getValues()
https://developers.google.com/apps-script/reference/spreadsheet/range#getvalues
http://webapps.stackexchange.com/questions/39494/google-scripts-how-to-distinguish-empty-cells-from-zero

write to a cell
http://stackoverflow.com/questions/11334296/google-docs-script-set-cell-value

delay / sleep
http://stackoverflow.com/questions/11142109/how-to-use-utilities-sleep-function

number format
https://developers.google.com/apps-script/reference/spreadsheet/range#setnumberformatnumberformat

clear cells
https://developers.google.com/apps-script/reference/spreadsheet/range#clear

hide sheet
https://developers.google.com/apps-script/reference/spreadsheet/sheet#hidesheet

*/
```
