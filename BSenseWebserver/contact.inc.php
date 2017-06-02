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
	<input type="hidden" name="UUID" value="<?php echo $UUID; ?>">
	    
	<fieldset>
		<legend>Contact</legend>
		<span class="contact_icn">Contact</span>
			
		<label for="user-name">Your Name</label>
		<input type="text" id="user-name" name="name" class="field" title="Your name" value="Your name"
			onfocus="if(this.value=='Your name')this.value='';" onblur="if(this.value=='')this.value='Your name';"/>
		
		<label for="user-email">Your Email</label>
		<input type="text" id="user-email" name="email" class="field" title="Your email" value="Your email"
			onfocus="if(this.value=='Your email')this.value='';" onblur="if(this.value=='')this.value='Your email';"/>
		
		<label for="user-comment">Your comments</label>
		<textarea id="user-comment" name="message" class="field" cols="" rows="" title="Your comments"
			onfocus="if(this.value=='Your comments')this.value='';" onblur="if(this.value=='')this.value='Your comments';">Your comments</textarea>
		
		<p id="formstatus"></p>
		
		<input type="image" value="Submit&raquo;" id="submit" src="/images/submit.gif" />

	</fieldset>
</form>
