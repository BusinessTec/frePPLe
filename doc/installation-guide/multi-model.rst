=================================================
Configuring multiple models in the user interface
=================================================

FrePPLe supports working with multiple models in the same web application.

This setup can be useful for the following very typical use cases:

* | **What-if models to support scenario analysis.**
  | A planner can do all kinds of what-if analysis in a copy of the production model.

  .. image:: _images/whatif.png
   :width: 421
   :height: 415
   :alt: What-if scenario's

* **Separate models for separate product lines or different factories.**

  When the interaction between product divisions or plants is relatively low, it might
  be useful to create seperate models for the planners to work in. This allows their
  business processes, planner workflows and data to be more loosely coupled.

  .. image:: _images/multimodel.png
   :width: 421
   :height: 415
   :alt: Multiple models

The following steps are required to configure a multi-model setup.

* **Create additional database schemas**

  The database administrator needs to create an additional schema in the database for each model.

* **Update the djangosettings.py configuration file**

  The connection details of each schema need to be added as a seperate section for the DATABASES
  parameter in the file settings.py.

  For instance:
  ::

     DATABASES = {
     'default': {
       'ENGINE': 'django.db.backends.postgresql_psycopg2',
       'NAME': 'frepple',
       'USER': '',
       'PASSWORD': '',
       'HOST': '',
       'OPTIONS': {},
       'PORT': '',
       },
     'scenario1': {
       'ENGINE': 'django.db.backends.postgresql_psycopg2',
       'NAME': 'scenario1',
       'USER': '',
       'PASSWORD': '',
       'HOST': '',
       'OPTIONS': {},
       'PORT': '',
       },
     }

  Some guidelines need to be considered when setting up the schemas:

    * | The number of schemas in the web application is unlimited.
      | Extra schema's have NO impact on the performance of the user interface.
        Only the disk space used by the database will increase.

    * One of the schemas MUST be called ‘default’.

      All information on user logins, user permissions and user preferences
      and browser sessions are stored in this default schema.

    * Use short and unambiguous names for the additional schemas.

      Since the names will be used as a prefix in the URLs they should be short
      and can't contain any special characters.

      Good examples: ‘scenario1′, ‘plant1′...

      Bad examples: ‘admin’, ‘buffer’, ‘scenario/1′, names with
      non-ASCII characters, names with spaces...

    * For whatif scenario modelling the schemas all MUST use the same engine.

      It is not possible to mix SQLite and PostgreSQL schemas.

      If the command frepple_copy isn’t used different database engines could
      in theory be used, but for simplicity, consistency and maintainability
      reasons that’s probably not a good idea.

    * The databases can be located on different database servers or database
      instances, but this is not required.
      This could be useful for instance to avoid that users running large tasks
      on what-if scenarios impact the performance of the regular production model.

* **Initialize the new schema(s)**

  If not done yet, the default schema is initialized with the following command.
  It creates all tables, indices and other database objects.

  ::

     frepplectl migrate

  To load the demo data in this database you run:

  ::

     frepplectl loaddata demo

  To initialize the additional schemas you copy the default schema with the
  command below. The command can also be executed from the user interface in
  the execution screen: see :doc:`../user-guide/execute`

  ::

     frepplectl frepple_copy default my_schema

  The copy process might take a while for bigger datasets. If it takes too long,
  you should consider running the copy as an automated batch job during quiet hours.

* **Restart the web server**

  After a change in the djangosettings.py file, the web server needs to be restarted.
