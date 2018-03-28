/* $Id: README.txt 1774 2012-05-03 17:47:54Z kevin $ */

Expiring Private File Link module


What it does

Simply put, it provides an Expiring Private Field Link field formatter 
for any file fields that you define in your content type(s).



How to use it?

After installing the module, there will now be "Expiring Private File 
Link" field formatter for any file fields you have.

For example, say you want to add the option for users to attach a 
private file download to a blog entry.

First add a file field to the blog content type. When you pick the 
formatter from the select box, select "Expiring Private File Link".

That's it! Now when the file is displayed in the page, the actual uri 
of the file isn't shown. Instead the uri that's show to the user looks 
something like 
http://yoursite.com/epflink/c501358361bb5ce479ee851945775cf9



Link Expiration

Expiring Private File Links will expire after a set number of minutes.

After they expire, a new link will automatically be generated the next 
time the page is requested for display.
If a user clicks an expried link, they are given a message that the 
link is expired, and a new link is generated for them.

Links can also be set to expire after use (one-time links). If set to 
expire after use, then a new link is created every time the file is 
served up.

The expiration settings can be controlled in the module's config page.



Controlling Access to Expiring Private File Link files

Expiring Private File Link access is controlled through Drupal's 
permissions system.

If you want to only offer Expiring Private File Link files for download 
to registered users, for example, just set the access setting so that 
anonymous users don't have access to Expiring Private File Links. The 
link will still be displayed for the anon user, but when they click on 
it they will be redirected to a login page that tells them they must 
register or login to access the content.



Example

A working example can be found at 
http://www.energywebdevelopment.com/node/355
