----------------------------------
--  DROP AND SETUP OUR SCHEMA   --
----------------------------------

DROP SCHEMA IF EXISTS lbaw2423 CASCADE;
CREATE SCHEMA lbaw2423;
SET search_path TO lbaw2423;

----------------------------------
--        OUR DEFINITIONS       --
----------------------------------

CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username TEXT NOT NULL CONSTRAINT users_username_uk UNIQUE,
    name TEXT NOT NULL,
    email TEXT NOT NULL CONSTRAINT users_email_uk UNIQUE,
    password TEXT NOT NULL,
    photo TEXT,
    is_blocked BOOLEAN DEFAULT false
);


CREATE TABLE administrators (
    id SERIAL PRIMARY KEY,
    username TEXT NOT NULL CONSTRAINT administrator_username_uk UNIQUE,
    name TEXT NOT NULL,
    email TEXT NOT NULL CONSTRAINT administrator_email_uk UNIQUE,
    password TEXT NOT NULL,
    photo TEXT
);

CREATE TABLE projects (
    id SERIAL PRIMARY KEY,
    name TEXT NOT NULL,
    detail TEXT,
    is_archived BOOLEAN DEFAULT false
);

CREATE TABLE project_members (
    id SERIAL PRIMARY KEY,
    project_id INT NOT NULL REFERENCES projects (id),
    user_id INT REFERENCES users(id),
    is_favorite BOOLEAN DEFAULT false
);

CREATE TABLE project_coordinators (
    id SERIAL PRIMARY KEY,
    project_member_id INT NOT NULL REFERENCES project_members (id)
);

CREATE TABLE forum_messages (
    id SERIAL PRIMARY KEY,
    project_id INT NOT NULL REFERENCES projects(id),
    project_member_id INT REFERENCES project_members (id),
    send_date TIMESTAMP WITH TIME ZONE DEFAULT now() NOT NULL CHECK (now() >= send_date),
    message VARCHAR(10000) NOT NULL
);

CREATE TABLE tasks (
    id SERIAL PRIMARY KEY,
    project_id INT NOT NULL REFERENCES projects(id),
    title TEXT NOT NULL,
    description TEXT NOT NULL,
    project_member_id INT REFERENCES project_members (id),
    is_completed BOOLEAN DEFAULT false,
    due_date TIMESTAMP WITH TIME ZONE DEFAULT now() 
);

CREATE TABLE suggested (
    id SERIAL PRIMARY KEY,
    task_id INT NOT NULL REFERENCES tasks(id),
    project_member_id INT NOT NULL REFERENCES project_members (id)
);

CREATE TABLE task_comments (
    id SERIAL PRIMARY KEY,
    task_id INT NOT NULL REFERENCES tasks(id),
    project_member_id INT REFERENCES project_members (id),
    post_date TIMESTAMP WITH TIME ZONE DEFAULT now() NOT NULL CHECK (now() >= post_date),
    comment TEXT NOT NULL
);

CREATE TYPE NOTIF_TYPE AS ENUM ('coordinator_change', 'invitation_accepted', 'project_invitation');

CREATE TABLE notifications (
    id SERIAL PRIMARY KEY,
    notif NOTIF_TYPE NOT NULL,
    project_id INT NOT NULL REFERENCES projects (id),
    user_id INT NOT NULL REFERENCES users(id),
    sent_date TIMESTAMP WITH TIME ZONE DEFAULT now() NOT NULL
);

CREATE TYPE TASK_NOTIF_TYPE AS ENUM ('task_assign', 'task_completed');

CREATE TABLE task_notifications (
    id SERIAL PRIMARY KEY,
    notif TASK_NOTIF_TYPE NOT NULL,
    project_id INT NOT NULL REFERENCES projects (id),
    user_id INT NOT NULL REFERENCES users(id),
    task_id INT NOT NULL REFERENCES tasks(id),
    sent_date TIMESTAMP WITH TIME ZONE DEFAULT now() NOT NULL
);

----------------------------------
--           TRIGGERS           --
----------------------------------

CREATE OR REPLACE FUNCTION fn_at_least_one_coordinator() RETURNS trigger AS $$
BEGIN
    IF (
        SELECT COUNT(PM.user_id)
        FROM project_members AS PM, project_coordinators AS PC
        WHERE PC.project_member_id = PM.id
        AND PM.project_id = ( -- select the project we want to remove the coordinator from
            SELECT project_members.project_id
            FROM project_members, project_coordinators
            WHERE project_members.id = OLD.project_member_id
            LIMIT 1
        )
    ) = 0 THEN
        RAISE EXCEPTION 'ERROR: Must be at least one project coordinator per project.';
        INSERT INTO project_coordinators VALUES (OLD.*);
    END IF;
    RETURN NULL;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER at_least_one_coordinator
AFTER DELETE ON project_coordinators
FOR EACH ROW
EXECUTE FUNCTION fn_at_least_one_coordinator();

CREATE OR REPLACE FUNCTION fn_raise_project_full_error() RETURNS trigger AS $$
BEGIN
    IF (
        SELECT COUNT(user_id)
        FROM project_members
        WHERE project_id = NEW.project_id
        AND user_id IS NOT NULL
    ) > 100 THEN
        RAISE EXCEPTION 'ERROR: The project is already full.';
        DELETE FROM project_members WHERE id = NEW.id;
    END IF;
    RETURN NULL;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER max_project_members
AFTER INSERT ON project_members
FOR EACH ROW
EXECUTE FUNCTION fn_raise_project_full_error();

CREATE OR REPLACE FUNCTION fn_task_limit() RETURNS trigger AS $$
BEGIN
    IF (
        SELECT COUNT(id)
        FROM tasks
        WHERE project_id = NEW.project_id
    ) > 5000 THEN
        RAISE EXCEPTION 'ERROR: The project cannot have more than 5000 tasks';
        DELETE FROM tasks WHERE id = NEW.id;
    END IF;
    RETURN NULL;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER task_limit
AFTER INSERT ON tasks
FOR EACH ROW
EXECUTE FUNCTION fn_task_limit();

CREATE OR REPLACE FUNCTION fn_notify_coordinator_change() RETURNS trigger AS $$
DECLARE
    u record;
    proj int;
BEGIN
    proj = (SELECT project_id FROM project_members WHERE id = NEW.project_member_id);
    for u in (SELECT user_id FROM project_members WHERE project_members.project_id = proj AND project_members.user_id IS NOT NULL) loop
        INSERT INTO notifications (notif, project_id, user_id) VALUES ('coordinator_change', proj, u.user_id);
    end loop;
    RETURN NULL;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER notify_coordinator_change
AFTER INSERT OR DELETE ON project_coordinators
FOR EACH ROW
EXECUTE FUNCTION fn_notify_coordinator_change();

CREATE OR REPLACE FUNCTION fn_notify_task_assign() RETURNS trigger AS $$
BEGIN
    INSERT INTO task_notifications (notif, project_id, user_id, task_id) VALUES ('task_assign', NEW.project_id, NEW.project_member_id, NEW.id);
    RETURN NULL;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER notify_task_assign
AFTER UPDATE ON tasks
FOR EACH ROW
WHEN (OLD.project_member_id <> NEW.project_member_id)
EXECUTE FUNCTION fn_notify_task_assign();

CREATE OR REPLACE FUNCTION fn_notify_task_completed() RETURNS trigger AS $$
DECLARE
    u record;
    proj int;
BEGIN
    proj = (SELECT project_id FROM tasks WHERE id = NEW.id);
    for u in (SELECT user_id FROM project_members WHERE project_members.project_id = proj AND project_members.user_id IS NOT NULL) loop
        INSERT INTO task_notifications (notif, project_id, user_id, task_id) VALUES ('task_completed', proj, u.user_id, NEW.id);
    end loop;
    RETURN NULL;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER notify_task_completed
AFTER UPDATE ON tasks
FOR EACH ROW
WHEN (NEW.is_completed = true AND OLD.is_completed = false)
EXECUTE FUNCTION fn_notify_task_completed();

----------------------------------
--     PERFORMANCE INDICES      --
----------------------------------

CREATE INDEX task_project ON tasks USING btree (project_id, project_member_id);
CLUSTER tasks USING task_project;

CREATE INDEX comment_task_time ON task_comments USING btree (task_id, post_date);
CLUSTER task_comments USING comment_task_time;

CREATE INDEX member_user ON project_members USING btree (user_id);
CLUSTER project_members USING member_user;

----------------------------------
--        SEARCH INDICES        --
----------------------------------
ALTER TABLE projects ADD COLUMN tsvectors TSVECTOR;

CREATE FUNCTION project_search_update() RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'INSERT' THEN
        NEW.tsvectors = to_tsvector('english', NEW.name);
    END IF;
    IF TG_OP = 'UPDATE' THEN
        IF (NEW.name <> OLD.name) THEN
            NEW.tsvectors = to_tsvector('english', NEW.name);
        END IF;
    END IF;
    RETURN NEW;
END $$
LANGUAGE plpgsql;

CREATE TRIGGER project_search_update
    BEFORE INSERT OR UPDATE ON projects
    FOR EACH ROW
    EXECUTE PROCEDURE project_search_update();

CREATE INDEX project_search_idx ON projects USING GIN (tsvectors);


ALTER TABLE users ADD COLUMN tsvectors TSVECTOR;

CREATE FUNCTION user_search_update() RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'INSERT' THEN
        NEW.tsvectors = (
            setweight(to_tsvector('english', NEW.username), 'A') ||
            setweight(to_tsvector('english', NEW.name), 'B')
        );
    END IF;
    IF TG_OP = 'UPDATE' THEN
        IF (NEW.username <> OLD.username OR NEW.name <> OLD.name) THEN
            NEW.tsvectors = (
                setweight(to_tsvector('english', NEW.username), 'A') ||
                setweight(to_tsvector('english', NEW.name), 'B')
            );
        END IF;
    END IF;
    RETURN NEW;
END $$
LANGUAGE plpgsql;

CREATE TRIGGER user_search_update
    BEFORE INSERT OR UPDATE ON users
    FOR EACH ROW
    EXECUTE PROCEDURE user_search_update();

CREATE INDEX user_search_idx ON users USING GIN (tsvectors);


ALTER TABLE tasks ADD COLUMN tsvectors TSVECTOR;

CREATE FUNCTION task_search_update() RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'INSERT' THEN
        NEW.tsvectors = to_tsvector('english', NEW.title);
    END IF;
    IF TG_OP = 'UPDATE' THEN
        IF (NEW.title <> OLD.title) THEN
            NEW.tsvectors = to_tsvector('english', NEW.title);
        END IF;
    END IF;
    RETURN NEW;
END $$
LANGUAGE plpgsql;

CREATE TRIGGER task_search_update
    BEFORE INSERT OR UPDATE ON tasks
    FOR EACH ROW
    EXECUTE PROCEDURE task_search_update();

CREATE INDEX task_search_idx ON tasks USING GIST (tsvectors);
