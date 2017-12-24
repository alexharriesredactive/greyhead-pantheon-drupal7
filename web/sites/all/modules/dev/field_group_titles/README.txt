Field groups shoud be setup normally, you can see the documentation here.
http://drupal.org/node/1017838

To override the title of a tab, fieldset, div, or accordion, go to content
types -> manage fields and add a new field with the type "Field Group Title".
Once the field is added, drag the field into the correct group, so it shows as
a child of the item it should correspond to. At this point when a user edits
the node they should now see the title field show up in the correct field
group.

Next, we need to add the title to the display, so go to content types ->
manage display. Here you can either drag and drop the field into the correct
position as a child of the group you wish to override, or you can scroll to
the bottom of the form, and there is an option named "Fieldgroups" where you
can clone the form view, meaning it would duplicate the structure seen in the
manage fields tab.

Now that the field you've created is set in the display, make sure to set the
label to HIDDEN. Hit save, and you should be able to edit the node, and rename
the tab to anything you want.
