A Security Analysis of Honeywords
=================================

Abstract
--------
Honeywords are decoy passwords associated with each user account, and they contribute a promising approach to detecting password leakage. This approach has been covered by hundreds of medias and also been adopted in various research domains. The idea of honeywords looks deceptively simple, but it is a deep and sophisticated challenge to automatically generate honeywords that are hard to differentiate from real passwords. In Juels-Rivest’s work, four main honeyword-generation methods are suggested but only justiﬁed by heuristic security arguments.

In this work, we for the ﬁrst time develop a series of experiments using 10 large-scale password datasets, a total of 104 million real-world passwords, to evaluate the security that these four methods can provide. Our results reveal that they all fail to provide the expected security: real passwords can be distinguished with a success rate of 29.29%∼32.62% by our basic trawling-guessing attacker, but not the claimed 5%, with just one guess (when each user account is associated with 19 honeywords as recommended). This ﬁgure reaches

34.21%∼49.02% under the advanced trawling-guessing attackers who make use of various state-of-the-art probabilistic password models. We further evaluate the security of Juels-Rivest’s methods under a targeted-guessing attacker who can exploit the victim’ personal information, and the results are even more alarming:

56.81%∼67.98%. Overall, our work resolves three open problems in honeyword research, as deﬁned by Juels and Rivest.

Experiments for Basic Trawling Attacks
--------------------------------------
As shown in `experimentsfor_basic_trawling_attacks.pdf`, we further evaluate Juels-Rivest’s four honeyword methods by using three datasets: Tianya, Rockyou and 000webhost.

An example of attacking code
--------------------------------------


A Security Analysis of Honeywords
---------------------------------
To achieve perfect honeyword-generation, Imran  proposed the honeyindex system. To generate honeywords for a given user, honeyindex directly uses the other users' passwords as this user's honeywords. So the distribution of honeywords is equal to the distribution of passwords.

### Distinguishing attack

There is a serious flaw in the Honeyindex system. If some sweetindex $si$ is contained  in only one user $U_i$'s sweetindex list, the password paired with $si$ will be $U_i$'s real password. This enables us to devise a method  to distinguish the real password:

1. Find an isolated sweetindex which appears only in *one* user's sweetindex list. It is certain that the password  pairing with this isolated sweetindex is  this user's  real password.

2. Delete all the incoming sweetindexes pairing with this user's password. As a result, new isolated sweetindexes will appear.

3. Go to Step 1 until no isolated sweetindex exists.

Note that, the sweetindex of user $U_i$'s real password cannot be contained in the sweetindex lists of users who registered before $U_i$, and it will be *only*  *potentially* contained in the  sweetindex lists of  users that registered later than $U_i$.Thus, the above distinguishing method can continue until all users' passwords are figured out.

Imran realized that "passwords of newly created accounts would not be used as honeywords", so he suggested regenerating sweetindexes periodically for all users. But another  problem arises: most users don't change their passwords periodically. So if the distinguishing attacker gets two password files that are leaked at different time points, the attacker only needs to compare a user's two sweetword lists in these two files, and can have a high confidence that the sweetword which is contained in both sweetword lists is the real password. Many websites don't realize that they  have been compromised until several years later. So the attacker has enough time to steal the password file   several times until  the website realizes this.

There is a another more serious problem in the honeyindex system. The passwords are stored in salted hash in the (sweetindex, password)--table. So it's hardly  impossible  to keep one user's sweetwords different from each other. If a user has a honeyword that is the same with his password, the position of the honeyword in the sweetword list will be sent to the honeychecker. This will cause a false alarm. To overcome  this problem, honeywords should be generated different from the real password. This can be done when the user registers, but it's  hardly  impossible when the sweetindex are regenerated periodically: the website does not know the plaintext of user's password, because all passwords have been hashed and salted.


### Heavy computational cost

The storage cost of the honeyindex system is less than the honeyword system, because honeyindex only stores one hash per user, while the honeyword system needs to store $k$ hashes per user. But the benefit of the honeyindex system is come at the cost of heavy computation.

The honeyword system uses the same salt for all the sweetwords of one user. When a user logins with password $pw$, the website only needs to compute the hash of $pw$ and compare the hash value with the hashes of $k$ sweetwords. But a user's sweetwords in the honeyindex system is hashed with different salt (because different accounts will, generally, have a different salt), so the website needs to compute $k$ times of slated-hash  at worst and $\frac{k}{2}$ hashes on average. Generally, it is recommended practice for the website to  use a slow hash function (e.g., bcrypt and PBKDF2) to securely store user passwords. As a result, the authentication time will be wasted at computing $k$ slow hashes. In all, the computational cost in a login of the honeyindex system is, on average,   $\frac{k}{2}$ times larger than that of the honeyword system.
