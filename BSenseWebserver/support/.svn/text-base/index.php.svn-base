<?php include("../header.inc.php"); ?>

<?php
// don't show the contact form on the sidebar:
define('SIDEBAR_DISABLE_FORM', 1);

// parse input
$UUID = htmlentities($_REQUEST['UUID']);
$version = htmlentities($_REQUEST['version']);
$OS = htmlentities($_REQUEST['OS']);

?>

<div id="inside" class="clearfloat">
	<div id="page-title" class="clearfloat">
	<h1>Support form</h1>
	</div>
	<div id="page-content">
	<!-- Content start -->

		<p>If you have any feedback about the project or you experienced a problem with
			the BSense application, please let us know using this form.</p>

			<script type="text/javascript">                                         
			/* <![CDATA[ */
				$(document).ready(function(){ 
					$("#contactform").submit(function(){
						var str = $(this).serialize();
						$.ajax({
						   type: "POST",
						   url: "/contact.act.php",
						   data: str,
						   success: function(msg) {
								$("#formstatus").ajaxComplete(function(event, request, settings){
									$(this).html(msg);
								});
							}
						 });
						return false;
					});
				});
			/* ]]> */	
			</script>  

			<form id="contactform" action="#">    
				<input type="hidden" name="UUID" value="<?php echo $UUID; ?>" />
				<input type="hidden" name="version" value="<?php echo $version; ?>" />
				<input type="hidden" name="OS" value="<?php echo $OS; ?>" />
				
				<fieldset>
					<legend>Contact</legend>
					<span class="contact_icn">Contact</span>

					<label for="user-name">Your Name</label>
					<input type="text" id="user-name" name="name" class="field-large" title="Your name" value="Your name"
						onfocus="if(this.value=='Your name')this.value='';" onblur="if(this.value=='')this.value='Your name';"/>

					<label for="user-email">Your Email</label>
					<input type="text" id="user-email" name="email" class="field-large" title="Your email" value="Your email"
						onfocus="if(this.value=='Your email')this.value='';" onblur="if(this.value=='')this.value='Your email';"/>

					<label for="user-comment">Your comments</label>
					<textarea id="user-comment-large" name="message" class="field-large" cols="" rows="" title="Your comments"
						onfocus="if(this.value=='Your comments')this.value='';" onblur="if(this.value=='')this.value='Your comments';">Your comments</textarea>

					<p id="formstatus"></p>

					<input type="image" value="Submit&raquo;" id="submit" src="/images/submit.gif" />

				</fieldset>
			</form>

	
	<!-- Content end -->
	</div>
	<?php include("../sidebar.inc.php"); ?>
</div>

<?php include("../footer.inc.php"); ?>
