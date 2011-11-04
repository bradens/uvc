CREATE TABLE Parts (
    pid integer,
    pname varchar(40),
    color varchar(20),
    PRIMARY KEY(pid)
);

CREATE TABLE PartsHistory (
    pid integer,
    pname varchar(40),
    color varchar(20),
    "Operation" char(1) NOT NULL,
    "When" TIMESTAMP NOT NULL,
    "User" CHAR(20) NOT NULL
);

INSERT INTO Parts Values(1, 'wrench', 'blue');
INSERT INTO Parts Values(2, 'wrench', 'red');
INSERT INTO Parts Values(3, 'key', 'yellow');

/***********************************************************************************************
                        Trigger Functions                                       
 ***********************************************************************************************/
CREATE OR REPLACE FUNCTION rowupdated() RETURNS TRIGGER
AS '
BEGIN
    INSERT INTO PartsHistory VALUES(NEW.pid, NEW.pname, NEW.color, ''U'', now(), CURRENT_USER);
    RETURN NULL;
END;
'
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION rowinserted() RETURNS TRIGGER
AS '
BEGIN
INSERT INTO PartsHistory VALUES(NEW.pid, NEW.pname, NEW.color, ''I'', now(), CURRENT_USER);
RETURN NULL;
END;
'
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION rowdeleted() RETURNS TRIGGER
AS '
BEGIN
INSERT INTO PartsHistory VALUES(OLD.pid, OLD.pname, OLD.color, ''D'', now(), CURRENT_USER);
RETURN NULL;
END;
'
LANGUAGE plpgsql;

/***********************************************************************************************
                        Trigger declarations                                       
***********************************************************************************************/
CREATE TRIGGER check_update 
    after update ON Parts
    FOR EACH ROW 
    EXECUTE PROCEDURE rowupdated();

CREATE TRIGGER check_insert 
after insert ON Parts
FOR EACH ROW 
EXECUTE PROCEDURE rowinserted();

CREATE TRIGGER check_delete 
BEFORE DELETE ON Parts
FOR EACH ROW 
EXECUTE PROCEDURE rowdeleted();

/***********************************************************************************************
                        Cleanup functions                                      
***********************************************************************************************/
DROP TRIGGER check_update on Parts;
DROP TRIGGER check_insert on Parts;
DROP TRIGGER check_delete on Parts;

DROP TABLE partshistory;
DROP TABLE parts;

