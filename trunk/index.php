<HTML>
<HEAD>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"> 
<META name="keywords" content="GRE Dictionary">
<META name="description" content="Saint Atique&#39;s GRE Definitions">
<TITLE>
GRE Words Definitions
</TITLE>
<?php
	include "./Vars.inc";
?>
<LINK rel="stylesheet" href="style.css" type="text/css">
<script language='javascript'>
function changeContent(tablecell)
{
    //alert(tablecell.firstChild.nodeValue);
    tablecell.innerHTML = "<INPUT type=text name=newname onBlur=\"javascript:submitNewName(this);\" value=\""+tablecell.innerHTML+"\">";
    tablecell.firstChild.focus();
}

function submitNewName(textfield)
{
    //alert(textfield.value);
    textfield.parentNode.innerHTML= textfield.value;
}
</script>
</HEAD><BODY>
<DIV id="wrapper"><br>
<DIV id="header"><?php
	echo "<a href=\"$homesite\">";
?>
<H1><?php
	echo $siteTitle;
?>
</H1>
</a></DIV>

<DIV id="nav1"><H3>Navigation</H3>
<?php
	echo "<A href=\"$ucpage\">";
?>Tech Support</A>
<br>
<?php
	echo "<a href=\"$homesite/acw/index.php\">";
?>Academic Works</a>
<br>
<?php
	echo "<a href=\"$ucpage\">";
?>Literature</a>
<br>
<?php
	echo "<a href=\"$ucpage\">";
?>About Me</a><br><br>
</DIV>

<DIV id="nav2">
<BR>
<H3>Sponsored Links</H3>
<A href="http://blog.saosx.com/">Computer Technology Support</A><BR>
<A href="http://sa-sdft.blogspot.com/">Saint Atique Blog</A><BR>
</DIV>
<DIV id="main"><DIV id="content">
<H2>Word List: High Frequency</H2>
<?php
	include "sa_gredic_db.php";
	$link_id = db_connect();
	
	if (!$link_id)
		die (sql_error());
	
	// Initialization
	$selectedpage = "";
	// Parse posted information
	// get page no
	if (isset($_POST['currentpage']))  {
		$selectedpage = $_POST["currentpage"];
	}
	if ($selectedpage == "")		
		$selectedpage = 0;
		
	$wordperpage = 20;
	$startlimit = $selectedpage * 20;
	$endlimit = $startlimit + $wordperpage;
	
	$query = "SELECT count(id) as totalWords FROM wordlist where wdtype = 1";
	// Perform Query
	$result = mysql_query($query);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}
	
	$totalWords = "";
	// Check result
	// This shows the actual query sent to MySQL, and the error. Useful for debugging.
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}

	if ($row = mysql_fetch_assoc($result)) {
		$totalWords = (int) $row['totalWords'];
	}
	
	// Free the resources associated with the result set
	// This is done automatically at the end of the script
	mysql_free_result($result);
	
	if ($totalWords == "")
		die("totalWords could not be retrieved.");

	if ($endlimit > $totalWords)
		$wordoffset = $totalWords - $startlimit;
	else
		$wordoffset = $wordperpage;
	// Next Query
	$query = "SELECT id, word, Meaning, Example, Antonym FROM wordlist where wdtype=1 LIMIT ".$startlimit.", ".$wordoffset;

	// Perform Query
	$result = mysql_query($query);
	
	// Make table
	echo "<table border=\"0\" id=\"maintable\">";

	$wordno = $startlimit;
	// Use result
	// Attempting to print $result won't allow access to information in the resource
	// One of the mysql result functions must be used
	// See also mysql_result(), mysql_fetch_array(), mysql_fetch_row(), etc.
	while ($row = mysql_fetch_assoc($result)) {
		$wordno = $wordno + 1;
		//echo "<tr><td onDblClick=\"javascript:changeContent(this);\">".$wordno.". ";

		echo "<tr><td width=\"30\">".$wordno.". </td><td id=\"wordtitle\">";
		echo $row['word']."</td></tr><tr><td onDblClick=\"javascript:changeContent(this);\" colspan=\"2\">";
		// display meaning
		if ($row['Meaning'] != "")
			//echo ucfirst($row['Meaning'])."<br>";
			echo ucfirst($row['Meaning'])."</td></tr><tr><td id=\"example\" colspan=\"2\">";

		// display Antonyms
		if ($row['Antonym'] != "")
			echo "Antonym: ".ucfirst($row['Antonym']);

		// display usage sentences
		$example = $row['Example'];
		if ($example != "") {
			$example = str_replace("<br>", "</li><li>", $example);
			
			echo "<ul style=\"list-style-type: square; padding: 0px; margin: 0px; text-indent: 0px; list-style-position: inside\"><li>".ucfirst($example)."</li></ul>";
		}
			
		//if ($wordno - $startlimit == 10)
			//echo "</td><td>";
		echo "</td></tr>";
	}

	// Free the resources associated with the result set
	// This is done automatically at the end of the script
	mysql_free_result($result);
	
	// end table
	echo "</table>";
	
	$pageTotalNo = $totalWords / $wordperpage;

	echo "<br><br><form action=\"\" method=\"post\"> 		<table border=\"0\" align=\"center\"> 		<td><font size=\"2\">Goto</font> 		</td> 		<td><select name=\"currentpage\">";

	for ($i = 0; $i < $pageTotalNo; $i++) {
		if ($selectedpage == ($i-1) % $pageTotalNo)
			echo "<option value = \"".$i."\" selected=\"true\">Next page";
		else
			echo "<option value = \"".$i."\">Page ".($i+1);
	}

	echo "</select>
		</td>
		<td>&nbsp;</td>
		<td><input type=\"submit\" value=\"Submit\" /></td>
		</tr>
		</table></form>";

?>

</DIV></DIV></div>

<DIV id="footer">SA GRE Dictionary 1.0</DIV>
</DIV>
</BODY></HTML>