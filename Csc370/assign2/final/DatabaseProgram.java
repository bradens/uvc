import java.sql.*;  
import java.text.*;
import java.io.*;

public class DatabaseProgram
{
    Connection   db;    
    Statement    sql;   
    DatabaseMetaData dbmd;  

    public DatabaseProgram(String argv[])
	throws ClassNotFoundException, SQLException
    {
	String database = argv[0];//"imdb";
	String username = argv[1];//"postgres";
	String password = argv[2];//"password";
	Class.forName("org.postgresql.Driver"); //load the driver
	db = DriverManager.getConnection("jdbc:postgresql:"+database, username, password);
	dbmd = db.getMetaData();
	System.out.println("Connection to "+dbmd.getDatabaseProductName()+" "+
			   dbmd.getDatabaseProductVersion()+" successful.\n");
	sql = db.createStatement();


	//For question 2o) the command line argument is one
	if(argv[3].equals("one"))
	    {

		String sqlText = "DROP TYPE moviesperyeartype CASCADE;";
		System.out.println("Executing this command: "+sqlText+"\n");
		sql.executeUpdate(sqlText);
		sqlText = "CREATE TYPE moviesperyeartype as (year2 character varying(50), count int8);";
		System.out.println("Executing this command: "+sqlText+"\n");
		sql.executeUpdate(sqlText);
		sqlText = "CREATE OR REPLACE FUNCTION DISCOVERYCHANNEL_moviesPerYear() " +
		    "RETURNS SETOF moviesperyeartype AS $$ SELECT year2, count(year2) as count " +
		    "FROM productions WHERE attr is null GROUP BY year2 ORDER BY year2 DESC $$ LANGUAGE SQL;";
		 
		System.out.println("Executing this command: "+sqlText+"\n");
		sql.executeUpdate(sqlText);
		sqlText = "select DISCOVERYCHANNEL_moviesPerYear();";
		System.out.println("Executing this command: "+sqlText+"\n");
		ResultSet results = sql.executeQuery(sqlText);
		if (results != null)
		    {
			System.out.println("Output");
			System.out.println("---------------------------------------");
			int i = 1;
			while (results.next())
			    {
				System.out.println(i+")  "+results.getString(1)+"\n");
				i++;
			    }
		    }

		sqlText = "DROP TYPE moviesperyeartype CASCADE;";
		System.out.println("Executing this command: "+sqlText+"\n");
		sql.executeUpdate(sqlText);
		results.close();
		db.close();

	    }
	//For question 2a) the command line argument is two
	else if(argv[3].equals("two"))
	    {
		String sqlText = "CREATE OR REPLACE FUNCTION DISCOVERYCHANNEL_clinteastwood() " +
		    "RETURNS SETOF VARCHAR(400) AS $$ SELECT id FROM roles NATURAL " +
		    "JOIN productions WHERE roles.pid = 'Eastwood, Clint' AND character !~ 'Himself' " +
		    "AND attr is NULL $$ LANGUAGE SQL;";
		System.out.println("Executing this command: "+sqlText+"\n");
		sql.executeUpdate(sqlText);

		sqlText = "select DISCOVERYCHANNEL_clinteastwood();";
		System.out.println("Executing this command: "+sqlText+"\n");
		ResultSet results = sql.executeQuery(sqlText);
		if (results != null)
		    {
			System.out.println("Output");
			System.out.println("---------------------------------------");
			int i = 1;
			while (results.next())
			    {
				System.out.println(i+")  "+results.getString(1)+"\n");
				i++;
			    }
		    }
		results.close();
		db.close();
	    }
    }



    public static void correctUsage()
    {
	System.out.println("\nIncorrect number of arguments.\nUsage:\n "+
			   "java   \n");
	System.exit(1);
    }

    public static void main (String args[])
    {
	//if (args.length != 3) correctUsage();
	try
	    {
		DatabaseProgram demo = new DatabaseProgram(args);
	    }
	catch (Exception ex)
	    {
		System.out.println("***Exception:\n"+ex);
		ex.printStackTrace();
	    }
    }
}