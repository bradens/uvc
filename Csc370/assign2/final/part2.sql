/************************ Part 2 ************************/
/************************ Q  1   ************************/
-- initializaton

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

-- functions

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

-- Declarations

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

-- Cleanup

--DROP TRIGGER check_update on Parts;
--DROP TRIGGER check_insert on Parts;
--DROP TRIGGER check_delete on Parts;

--DROP TABLE partshistory;
--DROP TABLE parts;

/************************ Q  2   ************************/

CREATE OR REPLACE FUNCTION DISCOVERYCHANNEL_trigger2() RETURNS trigger AS '
DECLARE
pnamelength integer;
colorlength integer;
BEGIN
pnamelength := length(NEW.pname);
colorlength := length(NEW.color);
pnamelength := pnamelength % 2;
colorlength := colorlength % 2;

IF pnamelength = 0 AND colorlength = 1 THEN
RAISE EXCEPTION ''pnames length is even, then so must be colors length'';
END IF;
RETURN NEW;
END;
' LANGUAGE plpgsql;

CREATE TRIGGER DISCOVERYCHANNEL_trigger2 BEFORE INSERT OR UPDATE ON Parts
FOR EACH ROW EXECUTE PROCEDURE DISCOVERYCHANNEL_trigger2();


/************************ Q  3   ************************/

CREATE OR REPLACE FUNCTION checkpname() RETURNS TRIGGER AS $$
BEGIN
IF NEW.pname !~ '[a-zA-Z]+-[0-9]+' then
RAISE EXCEPTION 
'pname does not meet format requirements. Reformat so that the part name follows the format part1-part2 where part1 is alphabetic, and part2 is numeric. For example: abc-124';
END IF;
RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER checkname BEFORE UPDATE OR INSERT ON parts 
FOR EACH ROW 
EXECUTE PROCEDURE checkpname();
