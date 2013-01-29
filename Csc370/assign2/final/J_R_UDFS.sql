
CREATE TYPE jtype as (P_ID varchar(200));

CREATE OR REPLACE FUNCTION discoverychannel_j() RETURNS setof jtype 
AS '
DECLARE 
    r jtype%rowtype;
BEGIN
for r in 
select pid from (select pid, avg(rank) as asrank from directors natural join (select id, rank, votes from ratings where votes >=100000) as foo natural join (select id, attr from productions where attr IS NULL) as bar GROUP BY directors.pid ORDER BY asrank DESC) as baz
LOOP
return next r;
end loop;
return;
END;
'
LANGUAGE plpgsql;

CREATE TYPE rtype as (ProductionTitle varchar(250), LinkCount bigint);

CREATE OR REPLACE FUNCTION discoverychannel_r() RETURNS setof rtype 
AS '
DECLARE
    r rtype%rowtype;
BEGIN
for r in
select title, count from (select count(*), title from productions join links on productions.id=links.idlinkedto where attr is NULL group by productions.id, title) as foo where count=(select max(count) from (select count(*), title from productions join links on productions.id=links.idlinkedto where attr is NULL group by productions.id, title order by count) as bar)
LOOP
    return next r;
END LOOP;
RETURN;
END;
'
LANGUAGE plpgsql;
